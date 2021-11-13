#!/bin/bash
#
# Copyright 2021 Collabora, Ltd.
#
# SPDX-License-Identifier: MIT
#
TARGET_DIR="$(realpath "$(dirname "$0")")"

docker build ${TARGET_DIR} -t native-example
