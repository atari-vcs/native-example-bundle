#pragma once

#include "haptic_effect.h"

#include <memory>

namespace controllers {

template<typename D>
class haptic_player {
public:
  virtual ~haptic_player() {}
  virtual void play_haptic_effect(std::shared_ptr<haptic_effect<D> const> effect) = 0;
  virtual void remove_haptic_effect(std::shared_ptr<haptic_effect<D> const> effect) = 0;
};

}
