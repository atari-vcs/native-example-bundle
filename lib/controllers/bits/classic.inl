#pragma once

#include "classic.h"

#include "axis_event.h"

namespace controllers {

inline classic::classic(SDL_Joystick *raw)
  : controller(SDL_JoystickInstanceID(raw)),
    twist_(0.005),
    haptic_(std::make_unique<haptic<single_rumble>>(SDL_HapticOpenFromJoystick(raw)))
{}

inline classic::~classic() {
  SDL_JoystickClose(raw_);
}

inline void classic::update(std::uint64_t dt, std::vector<std::unique_ptr<event>>& events) {
  controller::update(dt, events);

  double const change = twist_.update(dt);
  events.push_back(axis_event::motion(SDL_GetTicks(), shared_from_this(), axis::stick_twist, change));

  if( haptic_ ) {
    haptic_->update(dt);
  }
}

inline controller::kind classic::get_kind() const {
  return controller::kind::classic;
}

inline std::string classic::name() const {
  return SDL_JoystickName(raw_);
}

inline void classic::reopen() {
  raw_ = SDL_JoystickFromInstanceID(id());
  if( haptic_ ) {
    haptic_ = std::make_unique<haptic<single_rumble>>(SDL_HapticOpenFromJoystick(raw_));
  }
}

inline void classic::play_haptic_effect(std::shared_ptr<haptic_effect<single_rumble> const> effect) {
  if( haptic_ ) {
    haptic_->play_effect(effect);
  }
}

inline void classic::remove_haptic_effect(std::shared_ptr<haptic_effect<single_rumble> const> effect) {
  if( haptic_ ) {
    haptic_->remove_effect(effect);
  }
}

inline void classic::handle_event(SDL_Event const &evt, std::vector<std::unique_ptr<event>>& events) {
  switch( evt.type) {
  case SDL_JOYAXISMOTION:
    {
      auto axis = convert_axis(evt.jaxis.axis);
      auto value = convert_axis_value(evt.jaxis.axis, evt.jaxis.value);
      if( axis == axis::stick_twist ) {
        twist_.set_value(value);
      } else {
        events.push_back(axis_event::motion(evt.jaxis.timestamp, shared_from_this(), axis, value));
      }
    }
    break;

  case SDL_JOYBUTTONDOWN:
    events.push_back(button_event::down(
         evt.jbutton.timestamp,
         shared_from_this(),
         convert_button(evt.jbutton.button)));
    break;

  case SDL_JOYBUTTONUP:
    events.push_back(button_event::up(
         evt.jbutton.timestamp,
         shared_from_this(),
         convert_button(evt.jbutton.button)));
    break;

  case SDL_JOYHATMOTION:
    {
      std::pair<double,double> new_pos = hat_pos_;
      switch( evt.jhat.value ) {
      case SDL_HAT_LEFTUP:    new_pos = std::make_pair<double>(-1, -1); break;
      case SDL_HAT_UP:        new_pos = std::make_pair<double>( 0, -1); break;
      case SDL_HAT_RIGHTUP:   new_pos = std::make_pair<double>( 1, -1); break;
      case SDL_HAT_LEFT  :    new_pos = std::make_pair<double>(-1,  0); break;
      case SDL_HAT_CENTERED:  new_pos = std::make_pair<double>( 0,  0); break;
      case SDL_HAT_RIGHT:     new_pos = std::make_pair<double>( 1,  0); break;
      case SDL_HAT_LEFTDOWN:  new_pos = std::make_pair<double>(-1,  1); break;
      case SDL_HAT_DOWN:      new_pos = std::make_pair<double>( 0,  1); break;
      case SDL_HAT_RIGHTDOWN: new_pos = std::make_pair<double>( 1,  1); break;
      }
      if( new_pos.first != hat_pos_.first ) {
        events.push_back(axis_event::motion(
           evt.jhat.timestamp,
           shared_from_this(),
           convert_hat_x_axis(evt.jhat.hat),
           convert_hat_x_value(evt.jhat.hat, new_pos.first)));
      }
      if( new_pos.second != hat_pos_.second ) {
        events.push_back(axis_event::motion(
           evt.jhat.timestamp,
           shared_from_this(),
           convert_hat_y_axis(evt.jhat.hat),
           convert_hat_y_value(evt.jhat.hat, new_pos.second)));;
      }
      hat_pos_ = new_pos;
    }
    break;
  }
}

inline axis classic::convert_axis(std::uint8_t axis) const {
  switch( axis ) {
  case 0:
    return axis::stick_twist;
  default:
    return axis::invalid;
  }
}

inline double classic::convert_axis_value(std::uint8_t axis, std::int16_t value) const {
  switch( axis ) {
  case 0:
    // Twist axis
    return value / 32768.0;
  default:
    return 0;
  }
}

inline button classic::convert_button(std::uint8_t button) const {
  switch( button ) {
  case 0:  return button::a;
  case 1:  return button::b;
  case 2:  return button::back;
  case 3:  return button::menu;
  case 4:  return button::fuji;
  default: return button::invalid;
  }
}

inline axis classic::convert_hat_x_axis(std::uint8_t hat) const {
  if( hat == 0 ) {
    return axis::stick_x;
  }
  return axis::invalid;
}

inline axis classic::convert_hat_y_axis(std::uint8_t hat) const {
  if( hat == 0 ) {
    return axis::stick_y;
  }
  return axis::invalid;
}

inline double classic::convert_hat_x_value([[maybe_unused]] std::uint8_t hat, double value) const {
  return value;
}

inline double classic::convert_hat_y_value([[maybe_unused]] std::uint8_t hat, double value) const {
  return value;
}

}
