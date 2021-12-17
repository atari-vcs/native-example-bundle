#pragma once

#include "axis_event.h"
#include "button_event.h"
#include "controller.h"
#include "haptic.h"
#include "haptic_device_types.h"
#include "haptic_player.h"

#include <memory>

namespace controllers {

class generic: public controller,
               public haptic_player<dual_rumble>,
               public std::enable_shared_from_this<generic> {
public:
  explicit generic(SDL_GameController *raw);
  virtual ~generic();

  virtual std::string mapping() const;

public: // controller
  controller::kind get_kind() const override;
  std::string name() const override;
  void handle_event(SDL_Event const &evt, std::vector<std::unique_ptr<event>>& events) override;
  void reopen() final override;
  void update(std::uint64_t dt, std::vector<std::unique_ptr<event>>& events) override;

public: // haptic_player
  void play_haptic_effect(std::shared_ptr<haptic_effect<dual_rumble> const> effect) override;
  void remove_haptic_effect(std::shared_ptr<haptic_effect<dual_rumble> const> effect) override;

protected:
  virtual void on_reopen([[maybe_unused]] SDL_GameController *raw) {}
  virtual button convert_button(std::uint8_t button) const;
  virtual axis convert_axis(std::uint8_t axis) const;
  virtual double convert_axis_value([[maybe_unused]] std::uint8_t axis, std::int16_t value) const;
  virtual axis convert_dpad_to_axis(std::uint8_t button) const;
  virtual double convert_dpad_to_value(std::uint8_t button, int dir) const;

private:
  SDL_GameController *raw_;
  std::unique_ptr<haptic<dual_rumble>> haptic_;
};

}

#include "generic.inl"
