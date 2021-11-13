/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "event_handler.h"
#include "human_player.h"

namespace controllers {
  class controller;
  class event;
  class modern_controller;
}

/// A pad player is a player using a generic XBox360, or similar,
/// controller. Much of the logic is inherited by modern_pad_player.
class pad_player: public human_player {
public:
  pad_player(std::shared_ptr<car> controlled, std::shared_ptr<controllers::controller> ctrl);
public: // event_handler
  bool handle_event(std::shared_ptr<controllers::event const> evt);
private:
  std::shared_ptr<controllers::controller> ctrl_;
};
