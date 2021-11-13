/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "font.h"
#include "hiscore.h"
#include "race.h"
#include "render.h"
#include "title_screen.h"

#include <atari-controllers>

#include <SDL.h>

#include <iostream>

int main(int, char **) {
  if( SDL_Init(SDL_INIT_EVERYTHING) != 0 ) {
    std::cerr << "Failed to initialize SDL: "<<SDL_GetError() <<std::endl;
    std::exit(1);
  }
  SDL_GameControllerEventState(SDL_ENABLE);

  auto cs = std::make_shared<controllers::collection>();;

  font::init();

  render r("Native Example");

  std::vector<hiscore> hiscores = load_hiscores();

  bool done = false;
  while( !done ) {
    auto pads = title_screen(cs, r, hiscores);
    if( pads.empty() ) {
      std::exit(0);
    }
    if( !race(r, hiscores, pads, cs) ) {
      done = true;
    }
    save_hiscores(hiscores);
  }

  font::quit();
  SDL_Quit();

  std::exit(0);
}
