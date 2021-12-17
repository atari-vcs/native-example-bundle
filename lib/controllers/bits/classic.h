#pragma once

#include "axis_event.h"
#include "button_event.h"
#include "controller.h"
#include "haptic.h"
#include "haptic_device_types.h"
#include "haptic_player.h"
#include "radial_smoothing.h"

#include <cstdint>
#include <memory>

namespace controllers {

class classic: public controller,
               public haptic_player<single_rumble>,
               public std::enable_shared_from_this<classic>
{
public:
  explicit classic(SDL_Joystick *raw);
  virtual ~classic();

public: // controller
  controller::kind get_kind() const override;
  std::string name() const override;
  void reopen() final override;
  void handle_event(SDL_Event const &evt, std::vector<std::unique_ptr<event>>& events) override;
  void update(std::uint64_t dt, std::vector<std::unique_ptr<event>>& events) override;

public: // haptic_player
  void play_haptic_effect(std::shared_ptr<haptic_effect<single_rumble> const> effect) override;
  void remove_haptic_effect(std::shared_ptr<haptic_effect<single_rumble> const> effect) override;

protected:
  virtual void on_reopen([[maybe_unused]] SDL_Joystick *raw) {}
  virtual axis convert_axis(std::uint8_t axis) const;
  virtual double convert_axis_value(axis a, std::int16_t value) const;
  virtual button convert_button(std::uint8_t button) const;
  virtual axis convert_hat_x_axis(std::uint8_t hat) const;
  virtual axis convert_hat_y_axis(std::uint8_t hat) const;
  virtual double convert_hat_x_value(std::uint8_t hat, double value) const;
  virtual double convert_hat_y_value(std::uint8_t hat, double value) const;

private:
  SDL_Joystick *raw_;
  std::pair<double,double> hat_pos_;
  radial_smoothing twist_;
  std::unique_ptr<haptic<single_rumble>> haptic_;
};

}

#include "classic.inl"
