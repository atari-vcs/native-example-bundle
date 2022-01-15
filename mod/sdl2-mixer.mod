#!/bin/bash

export DEBIAN_FRONTEND=noninteractive
apt-get install -y --no-install-recommends git                                      

mkdir -p /usr/src
cd /usr/src
git clone -b release-2.0.4 https://github.com/libsdl-org/SDL_mixer.git

cd /usr/src/SDL_mixer
./configure --prefix=/usr
make -j$(nproc)
make install
