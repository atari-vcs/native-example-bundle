/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "title_screen.h"

#include "font.h"
#include "hiscore.h"
#include "math_helpers.h"
#include "model.h"
#include "render.h"
#include "render_helpers.h"

#include <atari-controllers>

#include <SDL.h>
#include <GL/gl.h>

#include <cmath>
#include <iostream>
#include <map>

enum class slot_state {
  empty,
  present,
  ready
};

static void draw_title(font const& f) {
  glColor3d(1, 1, 1);
  f.render_text_centered_to_width("Native Homebrew Example", vec2(0.5,0), 1);
}

static void draw_hiscores(font const &f, std::vector<hiscore> const &hs) {
  glColor3d(1, 1, 1);
  f.render_text_centered_to_height("BEST SCORES", vec2(0.5, 0.15), 0.05);
  for( unsigned i=0; i<hs.size(); ++i ) {
    auto & h = hs[i];
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0')
       << h.year() << "-"
       << std::setw(2) << std::setfill('0')
       << h.month() << "-"
       << std::setw(2) << std::setfill('0')
       << h.day()
       << " "
       << std::setw(2) << std::setfill('0')
       << h.hour() << ":"
       << std::setw(2) << std::setfill('0')
       << h.minute();
    glColor3d(1, 1, 1);
    f.render_text_to_height(ss.str(), vec2(0, 0.2 + i*0.08), 0.05);
    ss.str(std::string());
    ss << std::setw(4) << std::setfill('0')
       << h.score();
    glColor3d(1, 1, 0);
    f.render_text_right_to_height(ss.str(), vec2(1, 0.2 + i*0.08), 0.05);
  }
}

class wrapper {
public:
  virtual ~wrapper() {}
};

template<typename D>
class haptic_effect_wrapper: public wrapper {
public:
  explicit haptic_effect_wrapper(std::shared_ptr<controllers::haptic_player<D>> plyr,
                                 std::shared_ptr<controllers::haptic_effect<D> const> effect)
    : plyr_(plyr), effect_(effect)
  {
    plyr_->play_haptic_effect(effect_);
  }
  ~haptic_effect_wrapper() {
    plyr_->remove_haptic_effect(effect_);
  }
private:
  std::shared_ptr<controllers::haptic_player<D>> plyr_;
  std::shared_ptr<controllers::haptic_effect<D> const> effect_;
};

std::vector<std::shared_ptr<controllers::controller>>
title_screen(std::shared_ptr<controllers::collection> cs,
             render &r,
             std::vector<hiscore> const &hs)
{
  struct slot {
    slot()
      : ctrl(nullptr),
        state(slot_state::empty),
        angle(M_PI),
        angle_rate(0)
    {}

    std::shared_ptr<controllers::controller> ctrl;
    slot_state state;
    double angle;
    double angle_rate;
  };

  std::array<slot,8> slots;

  /* Put all the effects playing into a local vector, so we'll stop
     playing the effect on exit */
  std::vector<std::unique_ptr<wrapper>> wrappers;
  for( auto & c: *cs ) {
    switch(c->get_kind()) {
    case controllers::controller::kind::modern:
      {
        auto modern = std::dynamic_pointer_cast<controllers::modern>(c);
        auto effect = std::make_shared<controllers::dual_pulse_effect>(0.1, 0.2);
        wrappers.push_back(std::make_unique<haptic_effect_wrapper<controllers::dual_rumble>>(modern, effect));
      }
      break;

    case controllers::controller::kind::classic:
      {
        auto classic = std::dynamic_pointer_cast<controllers::classic>(c);
        auto effect = std::make_shared<controllers::single_tick_effect>(1.0, 1);
        wrappers.push_back(std::make_unique<haptic_effect_wrapper<controllers::single_rumble>>(classic, effect));
      }
      break;

    case controllers::controller::kind::generic:
      {
        auto generic = std::dynamic_pointer_cast<controllers::generic>(c);
        auto effect = std::make_shared<controllers::dual_pulse_effect>(0.1, 0.2);
        wrappers.push_back(std::make_unique<haptic_effect_wrapper<controllers::dual_rumble>>(generic, effect));
      }
      break;
    }
  }

  font f("res/CourierPrime-Regular.ttf", 300);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-7.0/18, (25.0/18), 1, 0, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  unsigned last_frame = SDL_GetTicks();
  bool quit = false;
  while( !quit ) {
    unsigned const this_frame = SDL_GetTicks();
    double const elapsed = (this_frame - last_frame)/1000.0;
    last_frame = this_frame;

    SDL_Event evt;
    while( SDL_PollEvent(&evt) ) {
      if( cs->handle_event(evt) ) {
        continue;
      }

      switch( evt.type ) {
      /* DEBUG */
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYDOWN:
        switch( evt.key.keysym.sym ) {
        case SDLK_ESCAPE:
          quit = true;
          break;
        }
      }
    }

    cs->update(static_cast<std::uint64_t>(elapsed * 1e9));

    for( auto event: cs->get_events() ) {
      switch( event->get_kind() ) {
      case controllers::event::kind::device_add:
      case controllers::event::kind::device_remove:
        break;
      case controllers::event::kind::axis_motion:
        {
          std::shared_ptr<controllers::axis_event> axis =
            std::dynamic_pointer_cast<controllers::axis_event>(event);
          for( unsigned i=0; i<slots.size(); ++i ) {
            if( slots[i].state != slot_state::empty && slots[i].ctrl->id() == axis->source()->id() ) {
              if( axis->get_axis() == controllers::axis::left_stick_x ||
                  axis->get_axis() == controllers::axis::stick_x ) {
                slots[i].angle_rate = 180 * M_PI/180 * axis->get_value();
              } else if( axis->get_axis() == controllers::axis::stick_twist ) {
                slots[i].angle += 180 * M_PI/180 * axis->get_value();
              }
              break;
            }
          }
          break;
        }
      case controllers::event::kind::button_down:
        {
          std::shared_ptr<controllers::button_event> button =
            std::dynamic_pointer_cast<controllers::button_event>(event);
          switch( button->get_button() ) {
          case controllers::button::a:
            {
              bool found = false;
              for( unsigned i=0; i<slots.size(); ++i ) {
                if( slots[i].state != slot_state::empty && slots[i].ctrl->id() == button->source()->id() ) {
                  found = true;
                  break;
                }
              }
              if( !found ) {
                for( unsigned i=0; i<slots.size(); ++i ) {
                  if( slots[i].state == slot_state::empty ) {
                    slots[i].ctrl = button->source();
                    if( slots[i].ctrl ) {
                      slots[i].state = slot_state::present;
                    }
                    break;
                  }
                }
              }
            }
            break;

          case controllers::button::menu:
            {
              for( unsigned i=0; i<slots.size(); ++i ) {
                if( slots[i].state != slot_state::empty && slots[i].ctrl->id() == button->source()->id() ) {
                  slots[i].state = slot_state::ready;
                  break;
                }
              }
            }
            break;

          case controllers::button::b:
            {
              for( unsigned i=0; i<slots.size(); ++i ) {
                if( slots[i].state != slot_state::empty && slots[i].ctrl->id() == button->source()->id() ) {
                  if( slots[i].state == slot_state::ready ) {
                    slots[i].state = slot_state::present;
                  } else if( slots[i].state == slot_state::present ) {
                    slots[i].state = slot_state::empty;
                    slots[i].ctrl.reset();
                  }
                  break;
                }
              }
            }
          default:
            // All other buttons ignored in title screen
            break;
          }
          break;
        }
      case controllers::event::kind::button_up:
        {
          break;
        }
      }
    }

    for(unsigned i=0; i<slots.size(); ++i ) {
      if( slots[i].state != slot_state::empty ) {
        slots[i].angle = slots[i].angle + normalize_angle(slots[i].angle_rate * elapsed);
      }
    }

    glClear(GL_COLOR_BUFFER_BIT);
    draw_title(f);
    draw_hiscores(f, hs);

    model m;
    for( unsigned i=0; i<slots.size(); ++i ) {
      glPushMatrix();
      double const x = i < 4 ? -0.25 : 1;
      double const y = (i % 4) * 0.25;
      glTranslated(x, y, 0);
      glScaled(0.25, 0.25, 1);

      glColor3d(1, 1, 1);
      glBegin(GL_LINE_LOOP);
      circle_vertices(0.5, 0.5, 0.4, 50);
      glEnd();

      switch(slots[i].state) {
      case slot_state::empty:
        break;
      case slot_state::present:
        set_color(i);
        glPushMatrix();
        glTranslated(0.5, 0.5, 0);
        glScaled(0.5, 0.5, 1);
        glRotated(slots[i].angle * 180 / M_PI, 0, 0, 1);
        m.draw();
        glPopMatrix();
        break;
      case slot_state::ready:
        glColor3d(0.5, 0.5, 0.5);
        glBegin(GL_POLYGON);
        circle_vertices(0.5, 0.5, 0.4, 50);
        glEnd();

        set_color(i);
        glPushMatrix();
        glTranslated(0.5, 0.5, 0);
        glScaled(0.5, 0.5, 1);
        glRotated(slots[i].angle * 180 / M_PI, 0, 0, 1);
        m.draw();
        glPopMatrix();
        break;
      }
      glPopMatrix();
    }

    r.swap();

    unsigned present = 0;
    unsigned ready = 0;
    for( unsigned i=0 ;i<slots.size(); ++i ) {
      switch( slots[i].state ) {
      case slot_state::empty:
        break;
      case slot_state::present:
        present++;
        break;
      case slot_state::ready:
        present++;
        ready++;
        break;
      }
    }

    if( present > 0 && present == ready ) {
      break;
    }
  }

  std::vector<std::shared_ptr<controllers::controller>> res;
  if( !quit ) {
    for( unsigned i=0; i<slots.size(); ++i ) {
      if( slots[i].state == slot_state::ready ) {
        res.push_back(slots[i].ctrl);
      } else {
        res.emplace_back();
      }
    }
  }
  return res;
}
