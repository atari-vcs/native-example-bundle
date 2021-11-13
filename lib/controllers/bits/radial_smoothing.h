#pragma once

#include <iostream>
#include <optional>

namespace controllers {

class radial_smoothing {
public:
  radial_smoothing(double rc, double initial=0)
    : old_(initial), rc_(rc)
  {}

  double update(std::uint64_t dt);
  void set_value(double value) {
    new_ = value;
  }
  double get_value() const {
    return old_;
  }

private:
  double old_;
  std::optional<double> new_;
  double rc_;
};

inline double radial_smoothing::update(std::uint64_t dt_ns) {
  double const dt = dt_ns * 1e-9;
  if( new_ ) {
    /* This is a standard controller input damping procedure: each
       update we want to move a proportion of the way from the old
       input to the new input, to remove noise. RC is effectively a
       damping constant, and doing it this way allows us to account
       for dt not being fixed.
    */
    double const alpha = dt / (dt + rc_);

    /* There's an additional complication for radial axes, because
       they wrap at -1/+1. So we start by working out what the
       shortest direction is from the old value to the new, including
       the possibility of going through the discontinuity at +1/-1
    */
    double const d1 = *new_ - old_;
    double const d2 = d1 - std::copysign(2, d1);
    double const d = std::fabs(d1) < std::fabs(d2) ? d1 : d2;

    /* Now we know what the damped change was for this update.
    */
    double const change = alpha * d;

    /* Finally we need to update our stored value with the damped
       value, making sure to wrap it into the correct range.
    */
    old_ = *new_ + change;
    if( std::fabs(old_) > 1 ) {
      old_ -= std::copysign(2, old_);
    }
    new_ = std::nullopt;

    /* We always return the change, since it's only known at this
       instant */
    return change;
  }
  return 0;
}

}
