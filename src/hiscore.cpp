/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#include "hiscore.h"

#include "error.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

hiscore::hiscore(unsigned hiscore)
  : year_(0), month_(0), day_(0), hour_(0), minute_(0), score_(hiscore)
{
  std::time_t const now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm const local = *std::localtime(&now);

  year_   = (local.tm_year + 1900) % 100;
  month_  = local.tm_mon;
  day_    = local.tm_mday;
  hour_   = local.tm_hour;
  minute_ = local.tm_min;
}

std::filesystem::path get_homedir_path() {
  char const *home = std::getenv("HOME");
  if( !home ) {
    crash();
  }
  return std::filesystem::path(home);
}

std::filesystem::path get_hiscore_path() {
  std::filesystem::path homedir(get_homedir_path());
  return homedir / "hiscores.txt";
}

static void prune_hiscores(std::vector<hiscore>& scores, unsigned count) {
  while( scores.size() > count ) {
    scores.pop_back();
  }
}

void new_hiscore(std::vector<hiscore>& scores, unsigned score) {
  hiscore const h(score);
  scores.insert(std::upper_bound(scores.begin(), scores.end(), h, std::greater<>()), h);
  prune_hiscores(scores, 10);
}

std::vector<hiscore> load_hiscores() {
  std::filesystem::path const hiscores = get_hiscore_path();

  if( !std::filesystem::exists(hiscores) ) {
    return std::vector<hiscore>();
  }

  std::ifstream infile(hiscores);

  std::vector<hiscore> res;

  std::string line;
  while( std::getline(infile, line) ) {
    std::stringstream ss(line);
    unsigned date;
    unsigned time;
    unsigned score;

    ss >> date >> time >> score;

    unsigned const year   = (date / 10000) % 100;
    unsigned const month  = (date / 100) % 100;
    unsigned const day    = (date) % 100;
    unsigned const hour   = (time / 100) % 100;
    unsigned const minute = (time) % 100;

    res.emplace_back(year, month, day, hour, minute, score);
  }

  std::sort(res.begin(), res.end(), std::greater<>());
  prune_hiscores(res, 10);

  return res;
}

void save_hiscores(std::vector<hiscore> const & hs) {
  std::ofstream outfile(get_hiscore_path());

  for( auto h: hs ) {
    outfile << h << std::endl;
  }
}
