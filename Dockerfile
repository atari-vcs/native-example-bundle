#
# Copyright 2021 Collabora, Ltd.
#
# SPDX-License-Identifier: MIT
#
FROM debian:buster-slim
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get upgrade -y  && \
    apt-get install -y --no-install-recommends \
        pkg-config \
        cmake \
        build-essential \
        libsdl2-dev \
        libsdl2-ttf-dev \
        libsdl2-mixer-dev
