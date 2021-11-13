/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "sample.h"
#include "soundscape2d.h"

#include "math_helpers.h"
#include "vec2.h"

#include <cmath>
#include <cstdint>
#include <memory>

/// A localised sound emitter within a 2d soundscape.
class emitter2d {
public:
  emitter2d(std::shared_ptr<soundscape2d> soundscape)
    : soundscape_(soundscape), sample_(nullptr), channel_(-1), pos_(vec2::zero())
  {
    channel_ = soundscape_->request_channel();
  }

  ~emitter2d(){
    if( channel_ >= 0 ) {
      soundscape_->release_channel(channel_);
    }
  }

  void set_pos(vec2 const &pos) {
    pos_ = pos;
    localise(false);
  }

  vec2 const & pos() const {
    return pos_;
  }

  void play(std::shared_ptr<sample> sample) {
    sample_ = sample;
    localise(true);
    Mix_PlayChannel(channel_, sample_->raw(), 0);
  }

  bool is_playing() const {
    return Mix_Playing(channel_) != 0;
  }

private:
  void localise(bool force) {
    if( force || Mix_Playing(channel_) ) {
      double const listener_z = soundscape_->listener_dist();
      vec2 const listener_xy = soundscape_->listener_pos();
      rectangle const & extent = soundscape_->extent();

      /* Place source in 3d */
      double const w = extent.width();
      double const h = extent.height();
      double const dx = pos_.x() - listener_xy.x();
      double const dy = pos_.y() - listener_xy.y();
      double const distance = std::sqrt(dx*dx + dy*dy + listener_z*listener_z);
      double const max_distance = std::sqrt((h*h/4) + (w*w/4) + listener_z*listener_z);
      double const theta_rad = std::atan2(dx, listener_z);
      double const theta_wrapped = theta_rad + (theta_rad < 0 ? M_PI*2 : 0);
      std::int16_t const theta_deg = static_cast<std::int16_t>(rad2deg(theta_wrapped));
      std::uint8_t const d = static_cast<std::uint8_t>(255.0*distance/(max_distance*2));

      Mix_SetPosition(channel_, theta_deg, d);
    }
  }

  std::shared_ptr<soundscape2d> soundscape_;
  std::shared_ptr<sample> sample_;
  int channel_;
  vec2 pos_;
};
