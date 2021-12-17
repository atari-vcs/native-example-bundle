#pragma once

#include <iostream>
#include <optional>

namespace controllers {

class radial_smoothing {
public:
  radial_smoothing(double rc, std::int16_t initial=0)
    : old_(initial), compare_(initial), rc_(rc)
  {}

  std::int16_t update(std::uint64_t dt);

  void set_value(std::int16_t value) {
    if( std::abs(wrapped_sub(value, compare_)) > LOCAL_JITTER_CUTOFF ) {
      new_ = value;
    }
  }

  std::int16_t get_value() const {
    return old_;
  }

private:
  static constexpr std::int16_t LOCAL_JITTER_CUTOFF = 192;

  static std::int16_t wrapped_add(std::int16_t x, std::int16_t y) {
    return static_cast<std::int16_t>(static_cast<std::uint16_t>(x) + static_cast<std::uint16_t>(y));
  }

  static std::int16_t wrapped_sub(std::int16_t x, std::int16_t y) {
    return static_cast<std::int16_t>(static_cast<std::uint16_t>(x) - static_cast<std::uint16_t>(y));
  }

  std::int16_t old_;
  std::int16_t compare_;
  std::optional<std::int16_t> new_;
  double rc_;
};

inline std::int16_t radial_smoothing::update(std::uint64_t dt_ns) {
  double const dt = dt_ns * 1e-9;
  if( new_ ) {
    /* This is a standard controller input damping procedure: each
       update we want to move a proportion of the way from the old
       input to the new input, to remove noise. RC is effectively a
       damping constant, and doing it this way allows us to account
       for dt not being fixed.
    */
    double const alpha = dt / (dt + rc_);

    /* The controller input already a full range 16-bit input; we have
       to convert to unsigned for the wraparound to be defined
       behaviour, but that's the only complication here.
    */
    std::int16_t const d = wrapped_sub(*new_, old_);

    /* Now we know what the damped change was for this update.
    */
    std::int16_t const change = static_cast<std::int16_t>(alpha * d);

    /* Finally we need to update our stored value with the damped
       value, making sure to wrap it into the correct range.
    */
    old_ = wrapped_add(*new_, change);
    new_ = std::nullopt;

    /* We always return the change, since it's only known at this
       instant */
    return change;
  }
  return 0;
}

}
