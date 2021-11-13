#pragma once

#include "modern.h"

namespace controllers {

inline modern::modern(SDL_GameController *raw)
  : generic(raw)
{
}

inline controller::kind modern::get_kind() const {
  return controller::kind::modern;
}

}
