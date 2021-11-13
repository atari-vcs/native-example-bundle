#pragma once

#include "controller.h"

#include <SDL.h>

#include <memory>
#include <vector>

namespace controllers {

class collection {
public:
  typedef std::vector<std::shared_ptr<controller>>::const_iterator const_iterator;

  collection();

  const_iterator begin() {
    return controllers_.begin();
  }

  const_iterator end() {
    return controllers_.end();
  }

  std::shared_ptr<controller> find_device(SDL_JoystickID id);
  std::shared_ptr<controller const> find_device(SDL_JoystickID id) const;

  void update(std::uint64_t dt);

  std::vector<std::shared_ptr<event>> get_events();

public: // event_handler
  bool handle_event(SDL_Event const& evt);

private:
  void prune_old();

  std::vector<std::shared_ptr<controller>> controllers_;
  std::vector<std::weak_ptr<controller>> old_;
  std::vector<std::unique_ptr<event>> events_;
};

}

#include "collection.inl"

