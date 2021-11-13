#pragma once

#include "event.h"

#include <iostream>

namespace controllers {

enum class axis {
  invalid,
  left_stick_x,
  left_stick_y,
  right_stick_x,
  right_stick_y,
  left_trigger,
  right_trigger,
  dpad_x,
  dpad_y,
  stick_x,
  stick_y,
  stick_twist
};

class axis_event: public event {
public:
  static std::unique_ptr<axis_event> motion(std::uint32_t timestamp,
                                            std::shared_ptr<controller> w,
                                            axis a,
                                            double value)
  {
    return std::make_unique<axis_event>(event::kind::axis_motion, timestamp, w, a, value);
  }

  axis_event(event::kind k, std::uint32_t timestamp, std::shared_ptr<controller> w, axis a, double value)
    : event(k, timestamp, w), axis_(a), value_(value)
  {}

  axis get_axis() const {
    return axis_;
  }

  double get_value() const {
    return value_;
  }

private:
  axis axis_;
  double value_;
};

inline std::ostream& operator<<(std::ostream& os, axis const & axis) {
  switch( axis ) {
  case axis::invalid:       os << "invalid";       break;
  case axis::left_stick_x:  os << "left_stick_x";  break;
  case axis::left_stick_y:  os << "left_stick_y";  break;
  case axis::right_stick_x: os << "right_stick_x"; break;
  case axis::right_stick_y: os << "right_stick_y"; break;
  case axis::left_trigger:  os << "left_trigger";  break;
  case axis::right_trigger: os << "right_trigger"; break;
  case axis::dpad_x:        os << "dpad_x";        break;
  case axis::dpad_y:        os << "dpad_y";        break;
  case axis::stick_x:       os << "stick_x";       break;
  case axis::stick_y:       os << "stick_y";       break;
  case axis::stick_twist:   os << "stick_twist";   break;
  }
  return os;
}

}
