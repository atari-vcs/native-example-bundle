#pragma once

#include <cstdint>

namespace controllers {

template <typename D>
class haptic_effect {
public:
  virtual ~haptic_effect() {}
  virtual typename D::params update(std::uint64_t elapsed) const = 0;
  virtual std::uint64_t duration() const {
    return UINT64_MAX;
  }
};

}
