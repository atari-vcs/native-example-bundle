#!/bin/bash
#
# Copyright 2021 Collabora, Ltd.
#
# SPDX-License-Identifier: MIT
#
set -e

TARGET_DIR="$(realpath "$(dirname "$0")")"
BUILD_DIR="$(realpath "$(pwd)")"
DOCKER_ARGS=(--rm -it -v "${BUILD_DIR}:/build" -v "${TARGET_DIR}:/source" -w /build)

docker run "${DOCKER_ARGS[@]}" native-example:latest /source/make-bundle.sh /source "$@"
