/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "sample.h"
#include "soundscape2d.h"

#include <SDL_mixer.h>

#include <memory>

/// Non-localised sound source, played center, full volume.
class tannoy {
public:
  explicit tannoy(std::shared_ptr<soundscape2d> soundscape)
    : soundscape_(soundscape), sample_(nullptr), channel_(-1)
  {
    channel_ = soundscape_->request_channel();
    // Clear any positioning that might have been previously done
    Mix_SetPosition(channel_, 0, 0);
  }

  ~tannoy(){
    if( channel_ >= 0 ) {
      soundscape_->release_channel(channel_);
    }
  }

  void play(std::shared_ptr<sample> sample) {
    sample_ = sample;
    Mix_PlayChannel(channel_, sample_->raw(), 0);
  }

  bool is_playing() const {
    return Mix_Playing(channel_) != 0;
  }

private:
  std::shared_ptr<soundscape2d> soundscape_;
  std::shared_ptr<sample> sample_;
  int channel_;
};
