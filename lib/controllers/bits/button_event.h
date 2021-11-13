#pragma once

#include "event.h"

namespace controllers {

enum class button {
  invalid,
  a,
  b,
  x,
  y,
  lb,
  rb,
  lsb,
  rsb,
  menu,
  back,
  fuji
};

class button_event: public event {
public:
  static std::unique_ptr<button_event> down(std::uint32_t timestamp, std::shared_ptr<controller> w, button b) {
    return std::make_unique<button_event>(event::kind::button_down, timestamp, w, b);
  }

  static std::unique_ptr<button_event> up(std::uint32_t timestamp, std::shared_ptr<controller> w, button b) {
    return std::make_unique<button_event>(event::kind::button_up, timestamp, w, b);
  }

  button_event(event::kind k, std::uint32_t timestamp, std::shared_ptr<controller> w, button b)
    : event(k, timestamp, w), button_(b)
  {}

  button get_button() const {
    return button_;
  }

private:
  button button_;
};

inline std::ostream& operator<<(std::ostream& os, button const & button) {
  switch( button ) {
  case button::invalid: os << "invalid"; break;
  case button::a:       os << "a";       break;
  case button::b:       os << "b";       break;
  case button::x:       os << "x";       break;
  case button::y:       os << "y";       break;
  case button::lb:      os << "lb";      break;
  case button::rb:      os << "rb";      break;
  case button::lsb:     os << "lsb";     break;
  case button::rsb:     os << "rsb";     break;
  case button::menu:    os << "menu";    break;
  case button::back:    os << "back";    break;
  case button::fuji:    os << "fuji";    break;
  }
  return os;
}

}
