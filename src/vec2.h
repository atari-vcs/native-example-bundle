/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include <cmath>
#include <iostream>

/// A simple 2D vector, to avoid pulling in a much more extensive math
/// library just for this.
class vec2 {
public:
  vec2(double x, double y)
    : x_(x), y_(y)
  {}

  vec2(vec2 const &other)
    : x_(other.x_), y_(other.y_)
  {}

  static vec2 x_axis() {
    return vec2(1,0);
  }

  static vec2 y_axis() {
    return vec2(0,1);
  }

  static vec2 zero() {
    return vec2(0,0);
  }

  explicit vec2(double v)
    : x_(v), y_(v)
  {}

  double const& x() const {
    return x_;
  }
  double const& y() const {
    return y_;
  }
  double & x() {
    return x_;
  }
  double & y() {
    return y_;
  }
  double mag() const {
    return std::sqrt(x_*x_ + y_*y_);
  }
  double theta() const {
    return std::atan2(y_, x_);
  }

  double dot(vec2 const &other) const {
    return x_ * other.x_ + y_ * other.y_;
  }
  double cross(vec2 const &other) const {
    return x_ * other.y_ - y_ * other.x_;
  }

  vec2& clamp(vec2 const& min, vec2 const& max) {
    x_ = std::max(min.x(), std::min(max.x(), x_));
    y_ = std::max(min.y(), std::min(max.y(), y_));
    return *this;
  }

  vec2 clamped(vec2 const& min, vec2 const& max) const {
    vec2 p(*this);
    p.clamp(min, max);
    return p;
  }

  vec2& normalize() {
    double const m = mag();
    x_ /= m;
    y_ /= m;
    return *this;
  }

  vec2 normalized() const {
    vec2 p(*this);
    p.normalize();
    return p;
  }

  vec2& safe_normalize(vec2 const &fallback) {
    double const tol = 1e-9;
    double const m = mag();
    if( m < tol ) {
      *this = fallback;
    } else {
      x_ /= m;
      y_ /= m;
    }
    return *this;
  }

  vec2 safe_normalized(vec2 const &fallback) const {
    vec2 p(*this);
    p.safe_normalize(fallback);
    return p;
  }

  vec2& rotate(double angle) {
    double const x =  x_ * std::cos(angle) + y_ * std::sin(angle);
    double const y = -x_ * std::sin(angle) + y_ * std::cos(angle);
    x_ = x;
    y_ = y;
    return *this;
  }

  vec2 rotated(double angle) const {
    vec2 p(*this);
    p.rotate(angle);
    return p;
  }

  vec2& rotate(vec2 const &origin, double angle) {
    double const rx = x_ - origin.x_;
    double const ry = y_ - origin.y_;
    double const x =  rx * std::cos(angle) + ry * std::sin(angle);
    double const y = -rx * std::sin(angle) + ry * std::cos(angle);
    x_ = x + origin.x_;
    y_ = y + origin.y_;
    return *this;
  }

  vec2 rotated(vec2 const &origin, double angle) const {
    vec2 p(*this);
    p.rotate(origin, angle);
    return p;
  }

  vec2& operator=(vec2 const &other) {
    x_ = other.x_;
    y_ = other.y_;
    return *this;
  }
  vec2& operator+=(vec2 const &other) {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
  }
  vec2& operator-=(vec2 const &other) {
    x_ -= other.x_;
    y_ -= other.y_;
    return *this;
  }
  vec2& operator*=(double scalar) {
    x_ *= scalar;
    y_ *= scalar;
    return *this;
  }

private:
  double x_;
  double y_;
};

static inline vec2 operator+(vec2 const& a, vec2 const &b) {
  vec2 p(a);
  p += b;
  return p;
}

static inline vec2 operator-(vec2 const& a, vec2 const &b) {
  vec2 p(a);
  p -= b;
  return p;
}

static inline vec2 operator*(vec2 const& a, double scalar) {
  vec2 p(a);
  p *= scalar;
  return p;
}

static inline vec2 operator*(double scalar, vec2 const& a) {
  vec2 p(a);
  p *= scalar;
  return p;
}

static inline std::ostream& operator<<(std::ostream& os, vec2 const &a) {
  os << "(" << a.x() << "," << a.y() << ")";
  return os;
}
