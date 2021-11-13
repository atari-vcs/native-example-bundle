#pragma once

#include <SDL.h>

namespace controllers {

/// This represents a device like the Atari Classic controller, which
/// is capable of dynamically updating effects, but which only has a
/// single motor. Since this is not well represented by SDL at
/// present, such devices usually report themselves as supporting the
/// left/right effect, even though really "left" would be a better
/// description.
class single_rumble {
public:
  typedef double params;

  static bool supported_by(SDL_Haptic *haptic) {
    SDL_HapticEffect effect;
    effect.leftright = {
      SDL_HAPTIC_LEFTRIGHT,
      SDL_HAPTIC_INFINITY,
      256,
      256
    };
    int res = SDL_HapticEffectSupported(haptic, &effect);
    if( res == SDL_TRUE ) {
      return true;
    }
    return false;
  }

  static int base_effect(SDL_Haptic *haptic) {
    SDL_HapticEffect effect;
    effect.leftright = {
      SDL_HAPTIC_LEFTRIGHT,
      SDL_HAPTIC_INFINITY,
      0,
      0
    };
    return SDL_HapticNewEffect(haptic, &effect);
  }

  static params clear() {
    return 0;
  }

  static void apply(SDL_Haptic *haptic, int effect, params params) {
    SDL_HapticEffect data;
    data.leftright.type = SDL_HAPTIC_LEFTRIGHT;
    data.leftright.length = SDL_HAPTIC_INFINITY;
    data.leftright.large_magnitude = static_cast<std::uint16_t>(std::max(0.0, std::min(1.0, params)) * 65535);
    data.leftright.small_magnitude = 0;
    SDL_HapticUpdateEffect(haptic, effect, &data);
  }
};

class dual_rumble {
public:
  struct params {
    double high;
    double low;
  };

  static bool supported_by(SDL_Haptic *haptic) {
    SDL_HapticEffect effect;
    effect.leftright = {
      SDL_HAPTIC_LEFTRIGHT,
      SDL_HAPTIC_INFINITY,
      256,
      256
    };
    int res = SDL_HapticEffectSupported(haptic, &effect);
    if( res == SDL_TRUE ) {
      return true;
    }
    return false;
  }

  static int base_effect(SDL_Haptic *haptic) {
    SDL_HapticEffect effect;
    effect.leftright = {
      SDL_HAPTIC_LEFTRIGHT,
      SDL_HAPTIC_INFINITY,
      0,
      0
    };
    return SDL_HapticNewEffect(haptic, &effect);
  }

  static params clear() {
    return { 0, 0 };
  }

  static void apply(SDL_Haptic *haptic, int effect, params params) {
    SDL_HapticEffect data;
    data.leftright.type = SDL_HAPTIC_LEFTRIGHT;
    data.leftright.length = SDL_HAPTIC_INFINITY;
    data.leftright.large_magnitude = static_cast<std::uint16_t>(std::max(0.0, std::min(1.0, params.low)) * 65535);
    data.leftright.small_magnitude = static_cast<std::uint16_t>(std::max(0.0, std::min(1.0, params.high)) * 65535);
    SDL_HapticUpdateEffect(haptic, effect, &data);
  }
};

}
