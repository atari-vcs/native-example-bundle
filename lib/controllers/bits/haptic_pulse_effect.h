#pragma once

#include "haptic_device_types.h"
#include "haptic_effect.h"

namespace controllers {

class single_tick_effect: public haptic_effect<single_rumble> {
public:
  explicit single_tick_effect(double magnitude=0.25, double frequency=1)
    : magnitude_(magnitude), frequency_(frequency)
  {}

  single_rumble::params update(std::uint64_t elapsed) const {
    double const seconds = elapsed * frequency_ * 1e-9;
    double junk;
    double const cycled = std::modf(seconds, &junk);
    double intensity = (std::sin(cycled * 2* M_PI) + 1)/2;
    return std::pow(intensity,100)*magnitude_;
  }

private:
  double magnitude_;
  double frequency_;
};

class dual_pulse_effect: public haptic_effect<dual_rumble> {
public:
  explicit dual_pulse_effect(double magnitude=0.25, double frequency=1, double phase_offset=0)
    : magnitude_(magnitude), frequency_(frequency), phase_offset_(phase_offset)
  {}

  dual_rumble::params update(std::uint64_t elapsed) const {
    double const seconds = elapsed * frequency_ * 1e-9;
    double junk;
    double const cycled = std::modf(seconds, &junk);
    double high_intensity = (std::sin(cycled * 2* M_PI) + 1)/2;
    double low_intensity = (std::sin(cycled * 2* M_PI + phase_offset_) + 1)/2;
    return dual_rumble::params { high_intensity*magnitude_, low_intensity*magnitude_ };
  }

private:
  double magnitude_;
  double frequency_;
  double phase_offset_;
};

}
