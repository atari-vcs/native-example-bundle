#pragma once

#include "generic.h"

#include <iostream>

namespace controllers {

inline generic::generic(SDL_GameController *raw)
  : controller(SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(raw))),
    raw_(raw),
    haptic_(nullptr)
{
  auto js = SDL_GameControllerGetJoystick(raw);
  if( SDL_JoystickIsHaptic(js) == SDL_TRUE ) {
    auto * h = SDL_HapticOpenFromJoystick(js);
    if( dual_rumble::supported_by(h) ) {
      haptic_ = std::make_unique<haptic<dual_rumble>>(h);
    } else {
      SDL_HapticClose(h);
    }
  }
}

inline generic::~generic() {
  SDL_GameControllerClose(raw_);
}

inline std::string generic::mapping() const {
  return SDL_GameControllerMapping(raw_);
}

inline controller::kind generic::get_kind() const {
  return controller::kind::generic;
}

inline std::string generic::name() const {
  return SDL_GameControllerName(raw_);
}

inline void generic::reopen() {
  raw_ = SDL_GameControllerFromInstanceID(id());
  if( haptic_ ) {
    auto js = SDL_GameControllerGetJoystick(raw_);
    haptic_ = std::make_unique<haptic<dual_rumble>>(SDL_HapticOpenFromJoystick(js));
  }
  on_reopen(raw_);
}

inline void generic::play_haptic_effect(std::shared_ptr<haptic_effect<dual_rumble> const> effect) {
  if( haptic_ ) {
    haptic_->play_effect(effect);
  }
}

inline void generic::remove_haptic_effect(std::shared_ptr<haptic_effect<dual_rumble> const> effect) {
  if( haptic_ ) {
    haptic_->remove_effect(effect);
  }
}

inline void generic::update(std::uint64_t dt, [[maybe_unused]] std::vector<std::unique_ptr<event>>& events) {
  if( haptic_ ) {
    haptic_->update(dt);
  }
}

inline void generic::handle_event(SDL_Event const &evt, std::vector<std::unique_ptr<event>>& events) {
  switch( evt.type ) {
  case SDL_CONTROLLERAXISMOTION:
    events.push_back(axis_event::motion(
         evt.caxis.timestamp,
         shared_from_this(),
         convert_axis(evt.caxis.axis),
         convert_axis_value(evt.caxis.axis, evt.caxis.value)));
    break;
  case SDL_CONTROLLERBUTTONDOWN:
  case SDL_CONTROLLERBUTTONUP:
    {
      switch(evt.cbutton.button ) {
      case SDL_CONTROLLER_BUTTON_DPAD_UP:
      case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
      case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
      case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        events.push_back(axis_event::motion(
             evt.cbutton.timestamp,
             shared_from_this(),
             convert_dpad_to_axis(evt.button.button),
             convert_dpad_to_value(evt.button.button, evt.type == SDL_CONTROLLERBUTTONDOWN ? 1 : -1)));
        break;

      default:
        if( evt.type == SDL_CONTROLLERBUTTONDOWN ) {
          events.push_back(button_event::down(
               evt.cbutton.timestamp,
               shared_from_this(),
               convert_button(evt.cbutton.button)));
        } else {
          events.push_back(button_event::up(
               evt.cbutton.timestamp,
               shared_from_this(),
               convert_button(evt.cbutton.button)));
        }
        break;
      }
    }
  }
}

inline button generic::convert_button(std::uint8_t button) const {
  switch( button ) {
  case SDL_CONTROLLER_BUTTON_A:             return button::a;
  case SDL_CONTROLLER_BUTTON_B:             return button::b;
  case SDL_CONTROLLER_BUTTON_X:             return button::x;
  case SDL_CONTROLLER_BUTTON_Y:             return button::y;
  case SDL_CONTROLLER_BUTTON_BACK:          return button::back;
  case SDL_CONTROLLER_BUTTON_GUIDE:         return button::fuji;
  case SDL_CONTROLLER_BUTTON_START:         return button::menu;
  case SDL_CONTROLLER_BUTTON_LEFTSTICK:     return button::lsb;
  case SDL_CONTROLLER_BUTTON_RIGHTSTICK:    return button::rsb;
  case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:  return button::lb;
  case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return button::rb;
  default:                                  return button::invalid;
  }
}

inline axis generic::convert_axis(std::uint8_t axis) const {
  switch( axis ) {
  case SDL_CONTROLLER_AXIS_LEFTX:        return axis::left_stick_x;
  case SDL_CONTROLLER_AXIS_LEFTY:        return axis::left_stick_y;
  case SDL_CONTROLLER_AXIS_RIGHTX:       return axis::right_stick_x;
  case SDL_CONTROLLER_AXIS_RIGHTY:       return axis::right_stick_y;
  case SDL_CONTROLLER_AXIS_TRIGGERLEFT:  return axis::left_trigger;
  case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: return axis::right_trigger;
  default:                               return axis::invalid;
  }
}

inline double generic::convert_axis_value([[maybe_unused]] std::uint8_t axis, std::int16_t value) const {
  return value / 32768.0;
}

inline axis generic::convert_dpad_to_axis(std::uint8_t button) const {
  switch( button ) {
  case SDL_CONTROLLER_BUTTON_DPAD_UP:    return axis::dpad_y;
  case SDL_CONTROLLER_BUTTON_DPAD_DOWN:  return axis::dpad_y;
  case SDL_CONTROLLER_BUTTON_DPAD_LEFT:  return axis::dpad_x;
  case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return axis::dpad_x;
  default:                               return axis::invalid;
  }
}

inline double generic::convert_dpad_to_value(std::uint8_t button, int dir) const {
  if( dir < 0 ) {
    return 0;
  }

  switch( button ) {
  case SDL_CONTROLLER_BUTTON_DPAD_UP:    return -1;
  case SDL_CONTROLLER_BUTTON_DPAD_DOWN:  return  1;
  case SDL_CONTROLLER_BUTTON_DPAD_LEFT:  return -1;
  case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return  1;
  default:                               return  0;
  }
}

}
