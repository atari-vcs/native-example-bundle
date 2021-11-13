/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "car.h"

#include "render_helpers.h"

#include <GL/gl.h>

#include <cmath>

car::car(vec2 const & pos, unsigned color, std::shared_ptr<model> model)
  : pos_(pos),
    theta_(0),
    vel_(0),
    throttle_(0),
    brake_(0),
    turn_(0),
    segment_(0xF),
    spin_timer_(0),
    spin_(0),
    score_(0),
    color_(color),
    model_(model)
{}

void car::draw() const {
  glPushMatrix();
  glTranslated(pos_.x(), pos_.y(), 0);
  glRotated(-theta_ * 180/M_PI, 0, 0, 1);
  set_color(color_);
  model_->draw();
  glPopMatrix();
}

template<typename T>
T clamp(T value, T min, T max) {
  return value < min ? min : value > max ? max : value;
}

/* returns an angle in [-M_PI, M_PI] */
static double normalize_angle(double theta) {
  double const rem = fmod(fmod(theta, 2*M_PI) + 2*M_PI, 2*M_PI);
  if( rem > M_PI ) {
    return rem - 2*M_PI;
  }
  return rem;
}

void car::update(double dt) {
  double const accel_scale = 14.4;
  double const turn_rate = 270 * (M_PI / 180); // 270 degrees per second
  double const lateral_friction = 50;
  double const rolling_friction = 0.24;
  double const braking_friction = 3.6;
  double const slip_limit = 20;

  vec2 const forward = vec2(0,1).rotate(theta_);
  vec2 const lateral = forward.rotated(M_PI/2);

  double const lateral_v = lateral.dot(vel_);
  double const forward_v = forward.dot(vel_);

  double const friction = -clamp(lateral_v * lateral_friction, -slip_limit, slip_limit);
  double const drag =
    -forward_v * rolling_friction - clamp(forward_v * brake_ * braking_friction, -slip_limit, slip_limit);

  vec2 const throttle = forward * (throttle_ * accel_scale);
  vec2 const resistance = friction * lateral + drag * forward;

  vec2 const accel = spin_ == 0 ? throttle + resistance : vec2::zero();

  pos_ += (vel_ + 0.5 * accel * dt) * dt;
  vel_ += accel * dt;

  if( spin_ ) {
    spin_timer_ -= dt;
    if( spin_timer_ <= 0 ) {
        spin_timer_ = 0;
        spin_ = 0;
    }
  }

  if( !spin_ ) {
    theta_ = normalize_angle(theta_ + turn_ * turn_rate * dt);
  } else {
    theta_ = normalize_angle(theta_ + spin_ * turn_rate * dt);
  }
}

circle car::collision_shape() const {
  return circle(pos_, model_->radius());
}

void car::set_collided(double speed, double angle) {
  if( spin_ ) {
    return;
  }
  if( std::fabs(speed) > 0.5 ) {
    spin_ = angle < 0 ? 2 : -2;
    spin_timer_ = 0.1*std::sqrt(std::fabs(speed));
  }
}
