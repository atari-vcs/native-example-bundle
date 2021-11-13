/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "circle.h"

#include <cmath>
#include <iostream>
#include <optional>
#include <memory>

/// A track or level. It's here to demonstrate loading resources
/// yourself from the unpacked bundle, and because a racing game needs
/// a track!
class level {
public:
  static std::shared_ptr<level> load(std::string const & filename);

  unsigned width() const {
    return w_;
  }

  unsigned height()  const {
    return h_;
  }

  bool blocker_at(unsigned x, unsigned y) const  {
    return get_raw(3, x, y) == 1;
  }

  bool spawn_at(unsigned x, unsigned y) const {
    return get_raw(3, x, y) == 2;
  }

  float steer_angle_at(unsigned x, unsigned  y) {
    unsigned const steps = get_raw(0, x, y);
    int const s = static_cast<int>((steps & 0x7) - (steps & 0x8));
    return s * M_PI / 8;
  }

  unsigned speed_at(unsigned x, unsigned y) {
    unsigned const value = get_raw(1, x, y);
    return value;
  }

  unsigned segment_at(unsigned x, unsigned y) {
    return static_cast<unsigned char>(get_raw(2, x, y));
  }

  unsigned char get_raw(unsigned layer, unsigned x, unsigned y) const {
    return (readmap_(x,y) >> (4*layer)) & 0xF;
  }

  void set_raw(unsigned layer, unsigned x, unsigned y, unsigned char value) {
    unsigned short const old = readmap_(x, y);
    unsigned short const masked = old & ~(0xF << (4*layer));
    unsigned short const replace = (value & 0xF) << (4*layer);
    unsigned short const newv = masked | replace;

    writemap_(x, y, newv);
  }

  level(std::unique_ptr<unsigned short[]>&& map, unsigned w, unsigned h)
    : map_(std::move(map)), w_(w), h_(h)
  {}

  void save(std::string filename) const;

  void draw() const;

  std::optional<circle> get_intersecting_shape(circle const &target) const;

private:
  static constexpr double obstacle_radius = 0.75/2;

  unsigned short readmap_(unsigned x, unsigned y) const {
    x = x < w_ ? x : (w_-1);
    y = y < h_ ? y : (h_-1);
    return map_[y*w_ + x];
  }

  void writemap_(unsigned x, unsigned y, unsigned short value) const {
    x = x < w_ ? x : (w_-1);
    y = y < h_ ? y : (h_-1);
    map_[y*w_ + x] = value;
  }

  std::unique_ptr<unsigned short[]> map_;
  unsigned w_;
  unsigned h_;
};
