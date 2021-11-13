/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include <memory>
#include <vector>

class hiscore;
class render;
namespace controllers {
  class controller;
  class collection;
}

/// Run one race, recording high scores. pads are the controllers
/// selected for use by players on the title screen, while cs is the
/// controller set which you need to keep updating to get controller
/// events.
bool race(render &r,
          std::vector<hiscore>& hiscores,
          std::vector<std::shared_ptr<controllers::controller>>& pads,
          std::shared_ptr<controllers::collection> cs);
