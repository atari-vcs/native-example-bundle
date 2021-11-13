#pragma once

#include "event.h"

namespace controllers {

class device_event: public event {
public:
  static std::unique_ptr<device_event> add(std::uint32_t timestamp, std::shared_ptr<controller> w) {
    return std::make_unique<device_event>(event::kind::device_add, timestamp, w);
  }
  static std::unique_ptr<device_event> remove(std::uint32_t timestamp, std::shared_ptr<controller> w) {
    return std::make_unique<device_event>(event::kind::device_remove, timestamp, w);
  }
  device_event(event::kind k, std::uint32_t timestamp, std::shared_ptr<controller> w)
    : event(k, timestamp, w)
  {}
};

}
