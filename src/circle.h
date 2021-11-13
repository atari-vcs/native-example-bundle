/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "vec2.h"

/// A simple bounding circle, used for cheap 2D collision detection.
class circle {
public:
  circle(vec2 const & center, double radius)
    : center_(center),
      radius_(radius)
  {}

  bool intersects(circle const & other) const {
    double const tol = 1e-5;
    return intersection_distance(other) >= tol;
  }

  // Positive for intersecting
  double intersection_distance(circle const &other) const {
    vec2 const delta = center_ - other.center_;
    double const gap = radius_ + other.radius_ - delta.mag();
    return gap;
  }

  vec2 separation_direction(circle const &other) const {
    return (center_ - other.center_).safe_normalized(vec2::y_axis());
  }

private:
  vec2 center_;
  double radius_;
};
