/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

/// A convenient 2d axis aligned area
class rectangle {
public:
  rectangle(vec2 const & top_left, vec2 const & bottom_right)
    : top_left_(top_left), bottom_right_(bottom_right)
  {}

  double width() const {
    return bottom_right_.x() - top_left_.x();
  }
  double height() const {
    return bottom_right_.y() - top_left_.y();
  }
  vec2 center() const {
    return (bottom_right_ + top_left_)*0.5;
  }
  vec2 top_left() const {
    return top_left_;
  }
  vec2 bottom_right() const {
    return bottom_right_;
  }
private:
  vec2 top_left_;
  vec2 bottom_right_;
};
