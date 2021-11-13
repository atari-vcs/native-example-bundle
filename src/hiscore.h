/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include <iomanip>
#include <iostream>
#include <vector>

/// A hiscore record. It doesn't include the player, but it does
/// contain the date and time, and the score achieved. It's really
/// just here so we have something to write, to demonstrate writing to
/// persistent storage.
class hiscore {
public:
  hiscore(unsigned hiscore);
  
  hiscore(unsigned char year, unsigned char month, unsigned char day,
          unsigned char hour, unsigned char minute, unsigned hiscore)
    : year_(year),
      month_(month),
      day_(day),
      hour_(hour),
      minute_(minute),
      score_(hiscore)
  {}

  unsigned year() const {
    return year_;
  }
  unsigned month() const {
    return month_;
  }
  unsigned day() const {
    return day_;
  }
  unsigned hour() const {
    return hour_;
  }
  unsigned minute() const {
    return minute_;
  }
  unsigned score() const {
    return score_;
  }

private:
  unsigned char year_;
  unsigned char month_;
  unsigned char day_;
  unsigned char hour_;
  unsigned char minute_;
  unsigned score_;
};


static inline std::ostream& operator<<(std::ostream& os, hiscore const &hs) {
  os << std::setw(2) << std::setfill('0')
     << hs.year()
     << std::setw(2) << std::setfill('0')
     << hs.month()
     << std::setw(2) << std::setfill('0')
     << hs.day()
     << " "
     << std::setw(2) << std::setfill('0')
     << hs.hour()
     << std::setw(2) << std::setfill('0')
     << hs.minute()
     << " "
     << std::setw(4) << std::setfill('0')
     << hs.score();
  return os;
}

static inline bool operator<(hiscore const &a, hiscore const &b) {
  return a.score() < b.score();
}

static inline bool operator>(hiscore const &a, hiscore const &b) {
  return a.score() > b.score();
}

void new_hiscore(std::vector<hiscore>& scores, unsigned score);
std::vector<hiscore> load_hiscores();
void save_hiscores(std::vector<hiscore> const & hs);
