/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "event_handler.h"
#include "player.h"

#include <memory>

class car;

/// Human players respond to input events, and they have a score we
/// care about.
class human_player: public player, public event_handler {
public: // player
  bool is_human() const { return true; }
  void update([[maybe_unused]] double dt) {}
  unsigned score() const;

protected:
  human_player(std::shared_ptr<car> controlled)
    : controlled_(controlled)
  {}

  std::shared_ptr<car> get_car() {
    return controlled_;
  }

  std::shared_ptr<car const> get_car() const {
    return controlled_;
  }
private:
  std::shared_ptr<car> controlled_;
};
