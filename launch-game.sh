#!/bin/sh
#
# Copyright 2021 Collabora, Ltd.
#
# SPDX-License-Identifier: MIT
#
set -x

P="$(dirname "$(busybox realpath "$0")")"

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${P}/lib"

"${P}/bin/native"
