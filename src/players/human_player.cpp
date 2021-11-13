/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "human_player.h"

#include "car.h"

unsigned human_player::score() const {
  return controlled_->score();
}
