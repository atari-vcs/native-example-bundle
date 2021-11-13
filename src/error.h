/*
* Copyright 2021 Collabora, Ltd.
*
* SPDX-License-Identifier: MIT
*/
#pragma once

#include <iostream>

/// Crash the game to get a coredump with the current state for
/// debugging purposes.
[[noreturn]]
static inline void crash(void) {
    std::cerr << "Attempting to dump core" << std::endl;
    (*(int *)nullptr) = 0;
    for( ;; );
}
