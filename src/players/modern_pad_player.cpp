/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "modern_pad_player.h"

#include "car.h"

#include <atari-controllers>

modern_pad_player::modern_pad_player(std::shared_ptr<car> controlled, std::shared_ptr<controllers::modern> ctrl)
  : pad_player(controlled, ctrl)
{
}
