#pragma once

#include "collection.h"

#include "classic.h"
#include "device_event.h"
#include "generic.h"
#include "modern.h"

#include <algorithm>
#include <optional>

namespace controllers {

inline std::shared_ptr<controller> make_controller(int device_index) {
  constexpr unsigned ATARI_MANUFACTURER_ID = 0x3250;
  constexpr unsigned ATARI_CLASSIC_CONTROLLER_ID = 0x1001;
  constexpr unsigned ATARI_MODERN_CONTROLLER_ID = 0x1002;

  if( SDL_JoystickGetDeviceVendor(device_index) == ATARI_MANUFACTURER_ID ) {
    // Atari controllers
    switch( SDL_JoystickGetDeviceProduct(device_index) ) {
    case ATARI_CLASSIC_CONTROLLER_ID:
      return std::make_shared<classic>(SDL_JoystickOpen(device_index));
    case ATARI_MODERN_CONTROLLER_ID:
      return std::make_shared<modern>(SDL_GameControllerOpen(device_index));
    }
  }
  // Controllers we don't have any special handling for
  if( SDL_IsGameController(device_index) ) {
    return std::make_shared<generic>(SDL_GameControllerOpen(device_index));
  }

  // Unsupported
  return std::shared_ptr<controller>();
}

inline std::vector<std::shared_ptr<controller>> get_controllers() {
  std::vector<std::shared_ptr<controller>> res;

  for( int i=0; i<SDL_NumJoysticks(); ++i ) {
    auto controller = make_controller(i);
    if( controller ) {
      res.push_back(controller);
    }
  }

  return res;
}

inline collection::collection()
  : controllers_(get_controllers())
{
}

inline void collection::prune_old() {
  for( auto iter = old_.begin(); iter != old_.end();  ) {
    if( iter->expired() ) {
      iter = old_.erase(iter);
    } else {
      ++iter;
    }
  }
}

inline std::optional<SDL_JoystickID> get_joystick_id(SDL_Event const &evt) {
  switch( evt.type ) {
  case SDL_CONTROLLERAXISMOTION:
    return evt.caxis.which;
  case SDL_CONTROLLERBUTTONDOWN:
  case SDL_CONTROLLERBUTTONUP:
    return evt.cbutton.which;
  case SDL_JOYAXISMOTION:
    return evt.jaxis.which;
  case SDL_JOYBALLMOTION:
    return evt.jball.which;
  case SDL_JOYHATMOTION:
    return evt.jhat.which;
  case SDL_JOYBUTTONDOWN:
  case SDL_JOYBUTTONUP:
    return evt.jbutton.which;
  }
  return std::nullopt;
}

inline bool collection::handle_event(SDL_Event const& evt) {
  switch( evt.type ) {
  case SDL_CONTROLLERDEVICEADDED:
    {
      auto new_controller = make_controller(evt.cdevice.which);
      prune_old();
      auto iter = std::find_if(old_.begin(), old_.end(),
                               [&](auto other) { return other.lock()->id() == new_controller->id(); });
      if( iter != old_.end() ) {
        new_controller.reset();
        controllers_.push_back(iter->lock());
        controllers_.back()->reopen();
        events_.push_back(device_event::add(evt.cdevice.timestamp, controllers_.back()));
        old_.erase(iter);
      } else {
        controllers_.push_back(new_controller);
        events_.push_back(device_event::add(evt.cdevice.timestamp, new_controller));
      }
    }
    return true;

  case SDL_CONTROLLERDEVICEREMOVED:
    {
      auto iter = std::find_if(controllers_.begin(), controllers_.end(),
                               [&](auto other) { return other->id() == evt.cdevice.which; });
      if( iter != controllers_.end() ) {
        old_.push_back(*iter);
        events_.push_back(device_event::remove(evt.cdevice.timestamp, *iter));
        controllers_.erase(iter);
      }
    }
    return true;

  case SDL_CONTROLLERDEVICEREMAPPED:
    {
      auto iter = std::find_if(controllers_.begin(), controllers_.end(),
                               [&](auto other) { return other->id() == evt.cdevice.which; });
      if( iter != controllers_.end() ) {
        (*iter)->reopen();
      }
    }
    return true;

  default:
    {
      auto maybe_j = get_joystick_id(evt);
      if( maybe_j ) {
        auto iter = std::find_if(controllers_.begin(), controllers_.end(),
                                 [&](auto other) { return other->id() == *maybe_j; });
        if( iter != controllers_.end() ) {
          (*iter)->handle_event(evt, events_);
        }
        return true;
      }
      break;
    }
  }
  return false;
}

inline void collection::update(std::uint64_t dt) {
  for( auto & c: controllers_ ) {
    c->update(dt, events_);
  }
}

inline std::vector<std::shared_ptr<event>> collection::get_events() {
  std::vector<std::shared_ptr<event>> res;
  for( auto & iter: events_ ) {
    std::shared_ptr<event> evt = std::move(iter);
    res.push_back(evt);
  }
  events_.clear();
  return res;
}

inline std::shared_ptr<controller> collection::find_device(SDL_JoystickID id) {
  auto iter = std::find_if(controllers_.begin(), controllers_.end(),
                           [&](auto c) { return c->id() == id; });
  if( iter != controllers_.end() ) {
    return *iter;
  }
  return std::shared_ptr<controller>();
}

inline std::shared_ptr<controller const> collection::find_device(SDL_JoystickID id) const {
  auto iter = std::find_if(controllers_.begin(), controllers_.end(),
                           [&](auto c) { return c->id() == id; });
  if( iter != controllers_.end() ) {
    return *iter;
  }
  return std::shared_ptr<controller>();
}

}
