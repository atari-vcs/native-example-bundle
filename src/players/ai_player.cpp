/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "ai_player.h"

#include "car.h"
#include "level.h"
#include "math_helpers.h"

#include <cmath>

unsigned ai_player::score() const {
  return controlled_->score();
}

void ai_player::update([[maybe_unused]] double dt)  {
  double const arrive_time = 0.1;

  vec2 const pos = controlled_->pos();

  unsigned const x = static_cast<unsigned>(pos.x());
  unsigned const y = static_cast<unsigned>(pos.y());

  double const theta_target = environment_->steer_angle_at(x, y);
  double const distance = angle_between(controlled_->theta(), theta_target);
  double const speed_target = environment_->speed_at(x, y) * 18 / 15.0;
  double const speed = std::cos(distance) * controlled_->vel().mag();

  if( speed > speed_target ) {
    controlled_->set_throttle(0);
    controlled_->set_brake(1);
  } else if (speed < speed_target ) {
    controlled_->set_throttle(1);
    controlled_->set_brake(0);
  } else {
    controlled_->set_throttle(0);
    controlled_->set_brake(0);
  }

  controlled_->set_turn(distance / arrive_time);
}
