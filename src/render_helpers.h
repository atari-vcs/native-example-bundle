/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include "error.h"

#include <GL/gl.h>

#include <cmath>
#include <iostream>

/// Set one of the 8 car colours as the GL draw color
inline void set_color(unsigned color) {
  switch(color & 7) {
  case 0: glColor3d(1.0, 0.0, 0.0); break; // Red
  case 1: glColor3d(0.0, 1.0, 0.0); break; // Green
  case 2: glColor3d(0.2, 0.2, 1.0); break; // Blue
  case 3: glColor3d(1.0, 0.0, 1.0); break; // Purple
  case 4: glColor3d(1.0, 1.0, 0.0); break; // Yellow
  case 5: glColor3d(0.0, 1.0, 1.0); break; // Cyan
  case 6: glColor3d(1.0, 1.0, 1.0); break; // White
  case 7: glColor3d(1.0, 0.5, 0.5); break; // Pink
  }
}

/// Call glVertex for a given number of points on a circle
inline void circle_vertices(double x, double y, double r, unsigned points) {
  double const step = 2 * M_PI / points;
  for( unsigned k=0; k<points; ++k ) {
    double const angle = k * step;
    glVertex3d( std::cos(angle) * r + x, std::sin(angle)*r + y, 0);
  }
}

/// Check for a GL error; if one happened, report it and crash
inline void check_gl_error_fn(const char *file, int line) {
  GLenum err;
  while( (err = glGetError()) != GL_NO_ERROR ) {
    std::cout << "Got error " << std::hex << err << " at "
              << file << ":"<<std::dec << line<<std::endl;
    crash();
  }
}

#define check_gl_error() check_gl_error_fn(__FILE__, __LINE__)
