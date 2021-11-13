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
  ~classic();

public: // controller
  controller::kind get_kind() const;
  std::string name() const;
  void reopen() final; //  FIXME: on_reopen
  void handle_event(SDL_Event const &evt, std::vector<std::unique_ptr<event>>& events);
  void update(std::uint64_t dt, std::vector<std::unique_ptr<event>>& events);

public: // haptic_player
  void play_haptic_effect(std::shared_ptr<haptic_effect<single_rumble> const> effect) override;
  void remove_haptic_effect(std::shared_ptr<haptic_effect<single_rumble> const> effect) override;

protected:
  virtual axis convert_axis(std::uint8_t axis) const;
  virtual double convert_axis_value(std::uint8_t axis, std::int16_t value) const;
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
