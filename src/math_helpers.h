/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include <cmath>

/// Convert radians to degrees
inline double rad2deg(double rad) {
  return rad * 180 / M_PI;
}

/// Convert degrees to radians
inline double deg2rad(double deg) {
  return deg * M_PI / 180;
}

/// Normalize an angle in radians to lie between +/- pi
inline double normalize_angle(double theta) {
  double const rem = fmod(fmod(theta, 2*M_PI) + 2*M_PI, 2*M_PI);
  if( rem > M_PI ) {
    return rem - 2*M_PI;
  }
  return rem;
}

/// Return the shorter, signed angle from theta1 to theta, accouting
/// for wraparound
inline double angle_between(double theta1, double theta2) {
  double const distance1 = theta2 - theta1;
  double const distance2 = std::copysign(2*M_PI - std::fabs(distance1), -distance1);
  return std::fabs(distance1) < std::fabs(distance2) ? distance1 : distance2;
}
