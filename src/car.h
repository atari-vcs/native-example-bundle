/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "circle.h"
#include "model.h"
#include "vec2.h"

#include <algorithm>
#include <memory>

/// A car on the track, with position and driving characteristics.
class car {
 public:
  car(vec2 const & pos, unsigned color, std::shared_ptr<model> model);

  void set_throttle(double throttle) {
    throttle_ = std::min(std::max(0.0, throttle), 1.0);
  }
  void set_turn(double rate) {
    turn_ = std::min(std::max(-1.0, rate), 1.0);
  }
  void set_brake(double brake) {
    brake_ = std::min(std::max(0.0, brake), 1.0);
  }
  void update(double dt);
  void draw() const;

  double throttle() const {
    return throttle_;
  }
  double turn() const {
    return turn_;
  }

  circle collision_shape() const;

  vec2 pos() const {
    return pos_;
  }
  vec2 vel() const {
    return vel_;
  }
  // theta increases anticlockwise
  double theta() const {
    return theta_;
  }
  void set_pos(vec2 const &pos) {
    pos_ = pos;
  }
  void set_vel(vec2 const &vel) {
    vel_ = vel;
  }
  void set_theta(double theta) {
    theta_ = theta;
  }

  unsigned segment() const {
    return segment_;
  }
  void set_segment(unsigned seg) {
    segment_ = seg;
  }

  void set_collided(double speed, double angle);

  unsigned score() const {
    return score_;
  }
  void add_score(unsigned amount) {
    score_ += amount;
  }

  unsigned color() const {
    return color_;
  }

 private:
  vec2 pos_;
  double theta_;
  vec2 vel_;
  double throttle_;
  double brake_;
  double turn_;
  unsigned segment_;
  double spin_timer_;
  int spin_;
  unsigned score_;
  unsigned color_;
  std::shared_ptr<model> model_;
};
