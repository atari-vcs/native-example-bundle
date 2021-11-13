/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include <SDL.h>

#include <memory>

/// This object owns our render context, so that it gets automatically
/// cleaned up on exit.
class render {
 public:
  render(std::string const &window_title);
  ~render();

  void swap();

 private:
  SDL_Window * win_;
  SDL_GLContext ctx_;
};
