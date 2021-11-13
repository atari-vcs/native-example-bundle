/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "pad_player.h"

#include "car.h"

#include <atari-controllers>

pad_player::pad_player(std::shared_ptr<car> controlled, std::shared_ptr<controllers::controller> ctrl)
  : human_player(controlled), ctrl_(ctrl)
{
}

bool pad_player::handle_event(std::shared_ptr<controllers::event const> evt) {
  switch( evt->get_kind() ) {
  case controllers::event::kind::axis_motion:
    if( evt->source() == ctrl_ ) {
      auto axis = std::dynamic_pointer_cast<controllers::axis_event const>(evt);
      switch( axis->get_axis() ) {
      case controllers::axis::left_stick_x:
        {
          double const turn = axis->get_value();
          get_car()->set_turn(-turn);
        }
        break;
      case controllers::axis::right_trigger:
        {
          double const throttle = axis->get_value();
          get_car()->set_throttle(throttle);
        }
        break;
      case controllers::axis::left_trigger:
        {
          double const brake = axis->get_value();
          get_car()->set_brake(brake);
        }
        break;
      default:
        break;
      }
      return true;
    }
    break;
  case controllers::event::kind::button_up:
  case controllers::event::kind::button_down:
    if( evt->source() == ctrl_ ) {
      return true;
    }
    break;
  default:
    break;
  }
  return false;
}
