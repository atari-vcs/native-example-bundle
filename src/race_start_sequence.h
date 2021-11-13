/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "race_audio.h"
#include "timer.h"

/// A little helper to track beeping three times before the race
/// starts
class race_start_sequence {
public:
  race_start_sequence(race_audio & audio)
    : seconds_(0),
      timer_(1),
      audio_(audio)
  {
    timer_.expire();
  }

  bool complete() const {
    return seconds_ == 4;
  }
  void update(double dt) {
    timer_.update(dt);
    if( timer_.complete() ) {
      seconds_++;
      if( seconds_ < 4 ) {
        audio_.play_starting_beep();
        timer_.reset();
      }
    }
  }
private:
  unsigned seconds_;
  timer timer_;
  race_audio & audio_;
};
