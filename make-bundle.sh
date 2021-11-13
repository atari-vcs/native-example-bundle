#!/bin/bash
#
# Copyright 2021 Collabora, Ltd.
#
# SPDX-License-Identifier: MIT
#
set -e

REQUIRED_LIBS=(
    libfluidsynth.so.1
    libjack.so.0
    libmodplug.so.1
    libmpg123.so.0
    libopus.so.0
    libopusfile.so.0
    libreadline.so.7
    libvorbisfile.so.3
    libSDL2_ttf-2.0.so.0
    libSDL2_mixer-2.0.so.0
)

cmake "$@"
make
mkdir -p lib

set +e
for lib in "${REQUIRED_LIBS[@]}"; do
    ldconfig -p | grep -F ${lib} | cut -d' ' -sf4 | xargs cp -t lib
    if [ $? -ne 0 ]; then
        echo  "Required library ${lib} not found in the build environment; install it there first." >&2
        exit 1
    fi
done

set -e
cpack -G ZIP

