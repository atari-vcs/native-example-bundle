/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

/// Count down elapsed time conveniently.
class timer {
public:
  timer(double duration)
    : elapsed_(0),
      duration_(duration)
  {}

  void expire() {
    elapsed_ = duration_;
  }

  void reset() {
    elapsed_ = 0;
  }

  void update(double dt) {
    elapsed_ += dt;
  }

  bool complete() const {
    return elapsed_ >= duration_;
  }
private:
  double elapsed_;
  double duration_;
};
