/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "model.h"

#include <GL/gl.h>

#include <cmath>
#include <iostream>

static double const w = 1.0;
static double const h = 1.2;

double model::radius() const {
  return std::max(w,h)/2;
}

void model::draw() const {
  glPushMatrix();
  glScaled(w, h, 1);

  glBegin(GL_QUADS);
  glVertex3d(-0.20, -0.5, 0);
  glVertex3d(-0.20,  0.5, 0);
  glVertex3d( 0.20,  0.5, 0);
  glVertex3d( 0.20, -0.5, 0);

  glVertex3d(-0.5,  -0.45 ,0);
  glVertex3d(-0.5,  -0.20, 0);
  glVertex3d(-0.22, -0.20, 0);
  glVertex3d(-0.22, -0.45, 0);

  glVertex3d( 0.5,  -0.45 ,0);
  glVertex3d( 0.5,  -0.20, 0);
  glVertex3d( 0.22, -0.20, 0);
  glVertex3d( 0.22, -0.45, 0);

  glVertex3d(-0.5,   0.00 ,0);
  glVertex3d(-0.5,   0.25, 0);
  glVertex3d(-0.22,  0.25, 0);
  glVertex3d(-0.22,  0.00, 0);

  glVertex3d( 0.5,   0.00 ,0);
  glVertex3d( 0.5,   0.25, 0);
  glVertex3d( 0.22,  0.25, 0);
  glVertex3d( 0.22,  0.00, 0);
  glEnd();

  glPopMatrix();
}
