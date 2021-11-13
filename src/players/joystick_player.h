/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "event_handler.h"
#include "human_player.h"

namespace controllers {
  class classic;
}

/// A joystick player is someone using an Atari Classic controller.
class joystick_player: public human_player {
public:
  joystick_player(std::shared_ptr<car> controlled, std::shared_ptr<controllers::classic> ctrl);
public: // event_handler
  bool handle_event(std::shared_ptr<controllers::event const> evt);
private:
  std::shared_ptr<controllers::classic> ctrl_;
};
