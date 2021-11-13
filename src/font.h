/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "vec2.h"

#include <memory>

class font_guts;

/// A font that can be used to render text to the screen.
class font {
public:
  static void init();
  static void quit();
  font(std::string const& filename, int point_size);
  ~font();
  void render_text_to_height(std::string const &str, vec2 const &pos, double h) const;
  void render_text_right_to_height(std::string const &str, vec2 const &pos, double h) const;
  void render_text_centered_to_width(std::string const &str, vec2 const &pos, double w) const;
  void render_text_centered_to_height(std::string const &str, vec2 const &pos, double h) const;
private:
  std::unique_ptr<font_guts> guts_;
};
