/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "font.h"

#include "render_helpers.h"

#include <GL/gl.h>
#include <SDL_ttf.h>

#include <cassert>

class font_guts {
public:
  font_guts(std::string const &filename, int point_size);
  ~font_guts();
  void render_text(std::string const &str, void (*fn)(SDL_Surface *))  const;

private:
  TTF_Font * font_;
};

void font::init() {
  if( TTF_Init() == -1 ) {
    std::cerr << "Failed to initialize SDL ttf: "<<TTF_GetError() <<std::endl;
    std::exit(1);
  }
}

void font::quit() {
  TTF_Quit();
}

font::font(std::string const &filename, int point_size)
  : guts_(std::make_unique<font_guts>(filename, point_size))
{}

font::~font() {
}

static void render_to_height(SDL_Surface *surf) {
  glBegin(GL_QUADS);
  glTexCoord2d(0, 0); glVertex3d(0, 0, 0);
  glTexCoord2d(1, 0); glVertex3d(surf->w / (double)surf->h, 0, 0);
  glTexCoord2d(1, 1); glVertex3d(surf->w / (double)surf->h, 1, 0);
  glTexCoord2d(0, 1); glVertex3d(0, 1, 0);
  glEnd();
  check_gl_error();
}

static void render_centered_to_height(SDL_Surface *surf) {
  double const x = surf->w/(2.0 * surf->h);
  glBegin(GL_QUADS);
  glTexCoord2d(0, 0); glVertex3d(-x, 0, 0);
  glTexCoord2d(1, 0); glVertex3d( x, 0, 0);
  glTexCoord2d(1, 1); glVertex3d( x, 1, 0);
  glTexCoord2d(0, 1); glVertex3d(-x, 1, 0);
  glEnd();
  check_gl_error();
}

static void render_right_to_height(SDL_Surface *surf) {
  double const x = surf->w/(double)surf->h;
  glBegin(GL_QUADS);
  glTexCoord2d(0, 0); glVertex3d(-x, 0, 0);
  glTexCoord2d(1, 0); glVertex3d( 0, 0, 0);
  glTexCoord2d(1, 1); glVertex3d( 0, 1, 0);
  glTexCoord2d(0, 1); glVertex3d(-x, 1, 0);
  glEnd();
  check_gl_error();
}

static void render_centered_to_width(SDL_Surface *surf) {
  glBegin(GL_QUADS);
  glTexCoord2d(0, 0); glVertex3d(-0.5, 0, 0);
  glTexCoord2d(1, 0); glVertex3d( 0.5, 0, 0);
  glTexCoord2d(1, 1); glVertex3d( 0.5, surf->h / (double)surf->w, 0);
  glTexCoord2d(0, 1); glVertex3d(-0.5, surf->h / (double)surf->w, 0);
  glEnd();
  check_gl_error();
}

void font::render_text_to_height(std::string const &str, vec2 const &pos, double h) const {
  glPushMatrix();
  glTranslated(pos.x(), pos.y(), 0);
  glScaled(h, h, 0);
  guts_->render_text(str, render_to_height);
  glPopMatrix();
}

void font::render_text_centered_to_width(std::string const &str, vec2 const &pos, double w) const {
  glPushMatrix();
  glTranslated(pos.x(), pos.y(), 0);
  glScaled(w, w, 0);
  guts_->render_text(str, render_centered_to_width);
  glPopMatrix();
}

void font::render_text_centered_to_height(std::string const &str, vec2 const &pos, double h) const {
  glPushMatrix();
  glTranslated(pos.x(), pos.y(), 0);
  glScaled(h, h, 0);
  guts_->render_text(str, render_centered_to_height);
  glPopMatrix();
}

void font::render_text_right_to_height(std::string const &str, vec2 const &pos, double h) const {
  glPushMatrix();
  glTranslated(pos.x(), pos.y(), 0);
  glScaled(h, h, 0);
  guts_->render_text(str, render_right_to_height);
  glPopMatrix();
}

font_guts::font_guts(std::string const &filename, int point_size)
  : font_(nullptr)
{
  font_ = TTF_OpenFont(filename.c_str(), point_size);
  if( !font_ ) {
    std::cerr << "Failed to load font: "<<TTF_GetError() <<std::endl;
    std::exit(1);
  }
}

font_guts::~font_guts() {
  if( font_ ) {
    TTF_CloseFont(font_);
  }
}

void font_guts::render_text(std::string const &str, void (*render)(SDL_Surface *surf)) const {
  SDL_Color color = { 255 , 255, 255, 255 };
  SDL_Surface *surf = TTF_RenderUTF8_Blended(font_, str.c_str(), color);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, texture);

  render(surf);

  glDeleteTextures(1, &texture);
  glDisable(GL_TEXTURE_2D);

  check_gl_error();

  SDL_FreeSurface(surf);
}
