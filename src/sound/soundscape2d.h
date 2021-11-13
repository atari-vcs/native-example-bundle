/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "rectangle.h"
#include "vec2.h"

#include <SDL_mixer.h>

#include <cstdlib>
#include <iostream>
#include <vector>

/// A representation of the whole soundscape of a game, in 2d.
class soundscape2d {
public:
  soundscape2d(rectangle const & extent, double listener_dist)
    : extent_(extent), listener_dist_(listener_dist), nchannels_(0)
  {
    if( Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16, 2, 1024) == -1 ) {
      std::cerr << "Failed to initialize SDL mixer: "<< Mix_GetError() << std::endl;
      std::exit(1);
    }
  }

  ~soundscape2d() {
    Mix_CloseAudio();
  }

  int request_channel() {
    if( free_channels_.empty() ) {
      /* Cost to mix is roughly linear I think; so exponential grow is
         inadvisable here */
      int const new_limit = nchannels_ + 1;
      Mix_AllocateChannels(new_limit);
      for( int i=nchannels_; i<new_limit; ++i ) {
        free_channels_.push_back(i);
      }
      nchannels_ = new_limit;
    }
    auto channel = free_channels_.back();
    free_channels_.pop_back();
    return channel;
  }

  void release_channel(int channel) {
    free_channels_.push_back(channel);
  }

  vec2 listener_pos() const {
    return extent_.center();
  }

  rectangle const & extent() const {
    return extent_;
  }
  double listener_dist() const {
    return listener_dist_;
  }

private:
  rectangle extent_;
  double listener_dist_;
  std::vector<int> free_channels_;
  int nchannels_;
};
