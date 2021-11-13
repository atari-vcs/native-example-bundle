/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "pad_player.h"

namespace controllers {
  class modern;
}

/// A modern pad player is someone using an Atari Modern
/// controller. This is mostly placeholder until it can be used to
/// show LED support for this controller.
class modern_pad_player: public pad_player {
public:
  modern_pad_player(std::shared_ptr<car> controlled, std::shared_ptr<controllers::modern> ctrl);
};
