/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include <memory>

namespace controllers {
  class event;
}

/// Base class for objects that can react to controller events.
class event_handler {
public:
  virtual ~event_handler() {}
  virtual bool handle_event(std::shared_ptr<controllers::event const> evt) =0;
};
