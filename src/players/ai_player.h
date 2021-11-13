/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "player.h"

#include <memory>

class car;
class level;

/// An AI player to make up the numbers, if less than 8 human players
/// are available.  It uses driving instructions baked into the track
/// to keep things simple.
class ai_player: public player {
public:
  ai_player(std::shared_ptr<car> controlled, std::shared_ptr<level> environment)
    :  controlled_(controlled),
       environment_(environment)
  {}
public: // player
  void update(double dt);
  bool is_human() const { return false; }
  unsigned score() const;
private:
  std::shared_ptr<car> controlled_;
  std::shared_ptr<level> environment_;
};
