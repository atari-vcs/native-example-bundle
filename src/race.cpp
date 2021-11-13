/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "race.h"

#include "car.h"
#include "error.h"
#include "font.h"
#include "hiscore.h"
#include "level.h"
#include "race_audio.h"
#include "race_start_sequence.h"
#include "render.h"
#include "render_helpers.h"
#include "timer.h"
#include "players/ai_player.h"
#include "players/joystick_player.h"
#include "players/modern_pad_player.h"
#include "players/pad_player.h"

#include <atari-controllers>

#include <GL/gl.h>

#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

static void init_projection(std::shared_ptr<level> lvl)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-7.0/18*lvl->width(), (25.0/18*lvl->width()), lvl->height(), 0, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void draw_scores(std::vector<std::shared_ptr<car>> const & cars, font const & f)
{
  unsigned const per_side = (cars.size() + 1)/2;

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-7.0/18, 25.0/18, 1, 0, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  for( std::size_t i=0; i<cars.size(); ++i ) {
    unsigned const side = i < per_side ? 0 : 1;
    unsigned const index = i % per_side;
    glPushMatrix();
    glScaled(1.0/ per_side, 1.0/per_side, 1);
    glTranslated(side ? 0 : per_side, index + 1 - (double)side, 0);
    glRotated(90, 0, 0, side ? 1 : -1);
    glScaled(0.25, 0.25, 1);
    glTranslated(1, 0, 0);

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << cars[i]->score();
    set_color(cars[i]->color());
    f.render_text_to_height(ss.str(), vec2(0, 0), 1);

    glPopMatrix();
  }

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
}

static void process_collisions(std::shared_ptr<level> lvl,
                               std::vector<std::shared_ptr<car>> &cars,
                               race_audio & audio) {
  for( ;; ) {
    bool collisions = false;
    for( auto c1: cars ) {
      circle c1_shape = c1->collision_shape();
      for( ;; ) {
        auto level_shape = lvl->get_intersecting_shape(c1_shape);
        if( !level_shape ) {
          break;
        }
        double const bounce = 0.2;
        vec2 const sep = c1_shape.separation_direction(*level_shape);
        /* move out of collision */
        double const closing_vel = -c1->vel().dot(sep);
        c1->set_pos(c1->pos() + sep * c1_shape.intersection_distance(*level_shape));
        c1->set_vel(c1->vel() + (1 + bounce) * closing_vel * sep);
        c1_shape = c1->collision_shape();
        audio.play_crash(c1->pos(), closing_vel);
        collisions = true;
      }

      /* Collisions with cars */
      for( auto c2: cars ) {
        if( c1 == c2 ) {
          continue;
        }
        circle const c2_shape = c2->collision_shape();
        if( c1_shape.intersects(c2_shape) ) {
          double const bounce = 0.5;
          vec2 const sep = c1_shape.separation_direction(c2_shape);
          /* move out of collision */
          double const distance = c1_shape.intersection_distance(c2_shape);
          c1->set_pos(c1->pos() + sep * distance * 0.5 );
          c2->set_pos(c2->pos() - sep * distance * 0.5 );

          /* bounce */
          double const closing_vel = c2->vel().dot(sep) - c1->vel().dot(sep);
          double const separating_vel = -closing_vel * bounce;
          double const dv = separating_vel - closing_vel;
          c1->set_vel(c1->vel() - dv * sep * 0.5);
          c2->set_vel(c2->vel() + dv * sep * 0.5);
          c1->set_collided(closing_vel, vec2::y_axis().rotated(c1->theta()).cross(sep));
          c2->set_collided(closing_vel, vec2::y_axis().rotated(c2->theta()).cross(sep));
          audio.play_crash(c1->pos(), closing_vel);

          collisions = true;
        }
      }
    }

    if( !collisions ) {
      break;
    }
  }
}

static void spawn_cars(std::vector<std::shared_ptr<car>> & cars, std::shared_ptr<level> lvl) {
  unsigned index = 0;

  for ( unsigned j=0; j<lvl->height(); ++j ) {
    for( unsigned i=0; i<lvl->width(); ++i ) {
      if( lvl->spawn_at(i, j) ) {
        if( index == cars.size() ) {
          return;
        }
        {
          const unsigned ix = index++;
          cars[ix]->set_pos( vec2(i +0.5, j+0.5));
          cars[ix]->set_vel( vec2(0,0));
          cars[ix]->set_theta( lvl->steer_angle_at(i,j));
        }
      }
    }
  }
}

static void score_car(std::shared_ptr<car> c, std::shared_ptr<level> lvl) {
  vec2 const pos = c->pos();
  unsigned const x = static_cast<unsigned>(pos.x());
  unsigned const y = static_cast<unsigned>(pos.y());
  unsigned const old_segment = c->segment();
  unsigned const new_segment = lvl->segment_at(x, y);
  if( new_segment != old_segment ) {
    if( new_segment == (old_segment + 1) % 0xE) {
      c->set_segment(lvl->segment_at(x,y));
      c->add_score(5);
    }
  }
}

bool race(render &r,
          std::vector<hiscore>& hiscores,
          std::vector<std::shared_ptr<controllers::controller>>& pads,
          std::shared_ptr<controllers::collection> cs)
{
  std::vector<std::shared_ptr<car>> cars;
  std::vector<std::shared_ptr<player>> players;
  std::vector<std::shared_ptr<event_handler>> event_handlers;


  font f2("res/CourierPrime-Regular.ttf", 300);

  auto lvl = level::load("res/track.dat");
  init_projection(lvl);
  race_audio audio(lvl);

  for( unsigned i=0; i<pads.size(); ++i ) {
    std::shared_ptr<car> c = std::make_shared<car>(vec2(1.5), i, std::make_shared<model>());
    cars.push_back(c);
    if( pads[i] ) {
      std::shared_ptr<human_player> player;
      switch( pads[i]->get_kind() ) {
      case controllers::controller::kind::modern:
        player = std::make_shared<modern_pad_player>(c, std::dynamic_pointer_cast<controllers::modern>(pads[i]));
        break;
      case controllers::controller::kind::classic:
        player = std::make_shared<joystick_player>(c, std::dynamic_pointer_cast<controllers::classic>(pads[i]));
        break;
      default:
        player = std::make_shared<pad_player>(c, pads[i]);
        break;
      }
      players.push_back(player);
      event_handlers.push_back(player);
    } else {
      auto ai = std::make_shared<ai_player>(c, lvl);
      players.push_back(ai);
    }
  }

  spawn_cars(cars, lvl);

  unsigned last_frame = SDL_GetTicks();
  race_start_sequence start(audio);
  timer match_timer(30);
  bool quit = false;
  for( ;; ) {
    unsigned const this_frame = SDL_GetTicks();
    double const elapsed = (this_frame - last_frame)/1000.0;
    last_frame = this_frame;

    if( !start.complete() ) {
      start.update(elapsed);
    } else {
      match_timer.update(elapsed);
      if( match_timer.complete() ) {
        break;
      }
    }

    SDL_Event evt;
    while( SDL_PollEvent(&evt) ) {
      if( !cs->handle_event(evt) ) {
        if( evt.type == SDL_QUIT ) {
          quit = true;
        } else if( evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE ) {
          quit = true;
        }
      }
    }
    cs->update(static_cast<std::uint64_t>(elapsed * 1e9));

    for( auto & event: cs->get_events() ) {
      for( auto c: event_handlers ) {
        if( c->handle_event(event) ) {
          break;
        }
      }
    }

    if( quit ) {
      break;
    }

    for( auto p: players ) {
      p->update(elapsed);
    }

    glClear(GL_COLOR_BUFFER_BIT);
    lvl->draw();

    /* Disable the cars while the starting beeps are sounding */
    if( start.complete() ) {
      for( auto c: cars ) {
        c->update(elapsed);
        score_car(c, lvl);
      }
    }

    process_collisions(lvl, cars, audio);

    for( auto c: cars ) {
      c->draw();
    }
    draw_scores(cars, f2);

    r.swap();
  }

  for( auto c: cars ) {
    std::cout << "Score: "<<c->color()<<": "<<c->score()<<std::endl;
  }

  for( auto p: players ) {
    if( p->is_human() ) {
      new_hiscore(hiscores, p->score());
    }
  }

  return !quit;
}
