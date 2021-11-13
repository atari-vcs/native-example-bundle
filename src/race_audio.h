/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "level.h"
#include "sound/emitter2d.h"
#include "sound/sample.h"
#include "sound/soundscape2d.h"
#include "sound/tannoy.h"
#include "vec2.h"

#include <memory>

/// A wrapper for the simple in-game audio.
class race_audio {
public:
  race_audio(std::shared_ptr<level> lvl)
    : soundscape_(std::make_shared<soundscape2d>(rectangle(vec2::zero(), vec2(lvl->width(), lvl->height())),
                                                 lvl->width())),
      beep_sound_(sample::load("res/beep.wav")),
      bash_sound_(sample::load("res/bash.wav")),
      crash_sound_(sample::load("res/crash.wav")),
      tannoy_(std::make_shared<tannoy>(soundscape_))
  {}

  void play_crash(vec2 const & pos, double vel) {
    if( vel < 5  ) {
      return;
    }
    for( auto & emitter: emitters_ ) {
      if( !emitter->is_playing() ) {
        emitter->set_pos(pos);
        if( vel < 10 ) {
          emitter->play(bash_sound_);
        } else {
          emitter->play(crash_sound_);
        }
        return;
      }
    }
    auto emitter = std::make_shared<emitter2d>(soundscape_);
    emitters_.push_back(emitter);
    emitter->set_pos(pos);
    emitter->play(crash_sound_);
  }

  void play_starting_beep() {
    tannoy_->play(beep_sound_);
  }

private:
  std::shared_ptr<soundscape2d> soundscape_;
  std::shared_ptr<sample> beep_sound_;
  std::shared_ptr<sample> bash_sound_;
  std::shared_ptr<sample> crash_sound_;
  std::shared_ptr<tannoy> tannoy_;
  std::vector<std::shared_ptr<emitter2d>> emitters_;
};
