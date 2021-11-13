/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "render.h"

#include "error.h"
#include "level.h"

#include <GL/gl.h>

static SDL_GLContext init_gl(SDL_Window *win) {
  SDL_GLContext ctx = SDL_GL_CreateContext(win);

  glShadeModel(GL_FLAT);
  glClearColor(0, 0, 0, 0);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  glViewport(0, 0, w, h);

  // Enable vsync
  SDL_GL_SetSwapInterval(1);

  return ctx;
}

render::render(std::string const &title)
  : win_(NULL), ctx_(NULL)
{
  win_ = SDL_CreateWindow(title.c_str(), 0, 0, 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
  if( !win_ ) {
    std::cerr << "Failed to create SDL window: "<<SDL_GetError() <<std::endl;
    crash();
  }
  ctx_ = init_gl(win_);
}

render::~render() {
  if( ctx_ ) {
    SDL_GL_DeleteContext(ctx_);
  }
  if( win_ ) {
    SDL_DestroyWindow(win_);
  }
}

void render::swap() {
  SDL_GL_SwapWindow(win_);
}
