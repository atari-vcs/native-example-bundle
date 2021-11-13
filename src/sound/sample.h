/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include <SDL_mixer.h>

#include <cstdlib>
#include <iostream>
#include <memory>

/// A sound sample to play.
class sample {
public:
  explicit sample(Mix_Chunk *raw)
    : raw_(raw)
  {}

  ~sample() {
    Mix_FreeChunk(raw_);
  }

  static std::shared_ptr<sample> load(std::string filename) {
    Mix_Chunk *raw = Mix_LoadWAV(filename.c_str());
    if( !raw ) {
      std::cerr << "Failed to load "<<filename<<": "<<Mix_GetError()<<std::endl;
      std::exit(1);
    }
    return std::make_shared<sample>(raw);
  }

  Mix_Chunk *raw() {
    return raw_;
  }
private:
  Mix_Chunk *raw_;
};
