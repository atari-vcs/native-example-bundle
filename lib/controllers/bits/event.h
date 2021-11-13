#pragma once

#include <cstdint>
#include <memory>

namespace controllers {

class controller;

class event {
public:
  enum class kind {
    device_add,
    device_remove,
    axis_motion,
    button_down,
    button_up,
  };

  virtual ~event() {}

  kind get_kind() const {
    return kind_;
  }
  std::uint32_t timestamp() const {
    return timestamp_;
  }
  std::shared_ptr<controller> source() const {
    return controller_;
  }

protected:
  event(event::kind k, std::uint32_t timestamp, std::shared_ptr<controller> w)
    : kind_(k), timestamp_(timestamp), controller_(w)
  {}

private:
  kind kind_;
  std::uint32_t timestamp_;
  std::shared_ptr<controller> controller_;
};

}
