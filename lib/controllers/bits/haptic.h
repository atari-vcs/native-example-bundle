#pragma once

#include "haptic_effect.h"

#include <SDL.h>

namespace controllers {

template <typename D>
class haptic {
public:
  explicit haptic(SDL_Haptic *haptic)
    : haptic_(haptic), effect_id_(-1)
  {
    effect_id_ = D::base_effect(haptic_);
    SDL_HapticRunEffect(haptic_, effect_id_, SDL_HAPTIC_INFINITY);
  }
  ~haptic() {
    if( haptic_ ) {
      if( effect_id_ >= 0 ) {
        SDL_HapticDestroyEffect(haptic_, effect_id_);
      }
      SDL_HapticClose(haptic_);
    }
  }

  void play_effect(std::shared_ptr<haptic_effect<D> const> effect) {
    effect_stack_.push_back(std::make_pair(effect, 0));
  }

  void remove_effect(std::shared_ptr<haptic_effect<D> const> effect) {
    for( auto iter = effect_stack_.rbegin(); iter != effect_stack_.rend(); ++iter ) {
      if( iter->first == effect ) {
        auto duration = iter->second;
        auto prev = std::next(iter);
        if( iter != effect_stack_.rbegin() ) {
          prev->second += duration;
        }
        effect_stack_.erase(prev.base());
        if( effect_stack_.empty() ) {
          apply(D::clear());
        }
        break;
      }
    }
  }

  void update(std::uint64_t dt) {
    bool const had_active_effects = effect_stack_.empty();
    std::uint64_t accumulator = dt;
    while( !effect_stack_.empty() ) {
      auto & pair = effect_stack_.back();
      auto & effect = pair.first;
      auto & duration = pair.second;
      duration += accumulator;
      if( duration < effect->duration() ) {
        apply(effect->update(duration));
        break;
      }
      accumulator = duration;
      effect_stack_.pop_back();
    }
    if( had_active_effects && effect_stack_.empty() ) {
      apply(D::clear());
    }
  }

private:
  void apply(typename D::params params) {
    D::apply(haptic_, effect_id_, params);
  }

  SDL_Haptic *haptic_;
  int effect_id_;
  std::vector<std::pair<std::shared_ptr<haptic_effect<D> const>,double>> effect_stack_;
};

}
