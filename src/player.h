/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

/// The base class for a player controlling a car in a race.
class player {
public:
  virtual ~player() {}
  virtual void update(double dt)  = 0;
  virtual bool is_human() const = 0;
  virtual unsigned score() const = 0;
};
