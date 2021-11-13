/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include <memory>
#include <vector>

class render;
class hiscore;
namespace controllers {
  class controller;
  class collection;
}

/// Display a title screen. This has two purposes: show off different
/// ways of interacting with controllers, and actually select which
/// controllers are in use.
std::vector<std::shared_ptr<controllers::controller>>
title_screen(std::shared_ptr<controllers::collection> cs,
             render &r,
             std::vector<hiscore> const &hs);
