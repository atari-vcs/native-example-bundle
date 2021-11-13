/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "level.h"

#include "error.h"
#include "render_helpers.h"

#include <cctype>
#include <fstream>
#include <iostream>

std::shared_ptr<level> level::load(std::string const &filename) {
  std::ifstream infile(filename);

  unsigned w, h;

  infile >> w >> h;

  std::unique_ptr<unsigned short[]> map(new unsigned short[w*h]);

  for( unsigned j=0; j<h; ++j ) {
    for( unsigned i=0; i<w; ++i ) {
      map[j*w + i] = 0;
    }
  }

  int layer = -2;
  unsigned row = 0;
  std::string line;
  while( std::getline(infile, line) ) {
    unsigned i = 0;

    while( i < line.length() && std::isspace(line[i]) ) {
      ++i;
    }

    /* Blank lines separate layers */
    if( i == line.length() ) {
      if( layer >= 0 && row < h ) {
        std::cerr << "Map layer "<<layer<<" was too short, aborting." << std::endl;
        crash();
      }
      row = 0;
      ++layer;
      continue;
    }

    if( layer < 0 ) {
      std::cerr << "Unexpected map data; expected a blank line after the dimensions" << std::endl;
      crash();
    }

    unsigned col = 0;
    for( ; i<line.length(); ++i ) {
      if( std::isspace(line[i]) ) {
        continue;
      }
      unsigned char value;
      if( line[i] >= '0'  && line[i] <= '9' ) {
        value = static_cast<unsigned char>(line[i] - '0');
      } else if( line[i] >= 'a'  && line[i] <= 'f' ) {
        value = static_cast<unsigned char>(line[i] - 'a' + 10);
      } else if( line[i] >= 'A'  && line[i] <= 'F' ) {
        value = static_cast<unsigned char>(line[i] - 'A' + 10);
      } else {
        std::cerr << "Map layer "<<layer<<" contains invalid character '"
                  <<line[i]<<"'at row "<<row<<" col "<<col<<std::endl;
        crash();
      }
      map[row*w + col] |= value << (layer * 4);
      ++col;
    }
    if( col < w ) {
      std::cerr << "Map layer "<<layer<<", row "<<row<<" was too short, aborting." << std::endl;
      crash();
    }
    ++row;
  }

  if( row < h ) {
    std::cerr << "Map layer "<<layer<<" was too short, aborting." << std::endl;
    crash();
  }

  if( layer < 3 ) {
    std::cerr << "Missing map layer " << (layer+1) << std::endl;
    crash();
  }

  return std::make_shared<level>(std::move(map), w, h);
}

void level::save(std::string filename) const {
  std::ofstream outfile(filename);

  outfile << w_ << " " << h_ << '\n';

  for( unsigned k=0; k<4; ++k) {
    outfile << '\n';
    for( unsigned i=0; i<h_; ++i ) {
      for( unsigned j=0; j<w_; ++j ) {
        unsigned value = static_cast<unsigned>(get_raw(k, j, i));
          outfile << std::hex << value;
      }
      outfile << '\n';
    }
  }
}

void level::draw() const {
  unsigned const points = 50;

  double const radius = obstacle_radius;

  for( unsigned i=0; i<w_; ++i ) {
    for( unsigned j=0; j<h_; ++j ) {
      if( blocker_at(i, j) ) {
        double const cell_x = i + 0.5;
        double const cell_y = j + 0.5;

        glColor3d(1.0, 1.0, 1.0);

        glBegin(GL_POLYGON);
        circle_vertices(cell_x, cell_y, radius, points);
        glEnd();

        glBegin(GL_LINE_LOOP);
        circle_vertices(cell_x, cell_y, radius, points);
        glEnd();
      }
    }
  }
}

std::optional<circle> level::get_intersecting_shape(circle const &target) const {
  for( unsigned i=0; i<w_; ++i ) {
    for( unsigned j=0; j<h_; ++j ) {
      if( blocker_at(i, j) ) {
        double const cell_x = i + 0.5;
        double const cell_y = j + 0.5;

        circle const obstacle(vec2(cell_x, cell_y), obstacle_radius);
        if( target.intersects(obstacle) ) {
          return obstacle;
        }
      }
    }
  }
  return std::nullopt;
}
