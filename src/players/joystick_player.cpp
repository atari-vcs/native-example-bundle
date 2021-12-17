/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "joystick_player.h"

#include "car.h"

#include <atari-controllers>

joystick_player::joystick_player(std::shared_ptr<car> controlled, std::shared_ptr<controllers::classic> ctrl)
  : human_player(controlled), ctrl_(ctrl)
{
}

bool joystick_player::handle_event(std::shared_ptr<controllers::event const> evt) {
  switch( evt->get_kind() ) {
  case controllers::event::kind::axis_motion:
    if( evt->source() == ctrl_ ) {
      auto axis = std::dynamic_pointer_cast<controllers::axis_event const>(evt);
      switch( axis->get_axis() ) {
      case controllers::axis::stick_x:
        get_car()->set_turn(-axis->get_value());
        break;
      case controllers::axis::stick_twist:
        get_car()->set_theta(get_car()->theta() - 720*(M_PI/180.0) * axis->get_value());
        break;
      default:
        break;
      }
      return true;
    }
    break;
  case controllers::event::kind::button_down:
    if( evt->source() == ctrl_ ) {
      auto button = std::dynamic_pointer_cast<controllers::button_event const>(evt);
      switch( button->get_button() ) {
      case controllers::button::a:
        get_car()->set_throttle(1);
        break;
      case controllers::button::b:
        get_car()->set_brake(1);
        break;
      default:
        break;
      }
      return true;
    }
    break;
  case controllers::event::kind::button_up:
    if( evt->source() == ctrl_ ) {
      auto button = std::dynamic_pointer_cast<controllers::button_event const>(evt);
      switch( button->get_button() ) {
      case controllers::button::a:
        get_car()->set_throttle(0);
        break;
      case controllers::button::b:
        get_car()->set_brake(0);
        break;
      default:
        break;
      }
      return true;
    }
    break;
  default:
    break;
  }
  return false;
}
