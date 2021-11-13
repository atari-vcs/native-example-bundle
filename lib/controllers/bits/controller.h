#pragma once

#include "event.h"

#include <SDL.h>

#include <cstdint>
#include <string>
#include <vector>

namespace controllers {

class controller {
public:
  enum class kind {
    generic,
    classic,
    modern
  };

  virtual ~controller() {}

  SDL_JoystickID id() const { return id_; }

  virtual void update([[maybe_unused]] std::uint64_t dt,
                      [[maybe_unused]] std::vector<std::unique_ptr<event>>& events)
  {}

  virtual std::string name() const = 0;
  virtual kind get_kind() const = 0;
  virtual void reopen() = 0;
  virtual void handle_event(SDL_Event const &evt, std::vector<std::unique_ptr<event>>& events_out) = 0;

protected:
  explicit controller(SDL_JoystickID id) : id_(id) {}

private:
  SDL_JoystickID id_;
};

}
