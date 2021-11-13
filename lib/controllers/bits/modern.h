#pragma once

#include "generic.h"

#include <memory>

namespace controllers {

class modern: public generic
{
public:
  explicit modern(SDL_GameController *raw);

public: // controller
  controller::kind get_kind() const;
};

}

#include "modern.inl"
