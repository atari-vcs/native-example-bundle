#!/bin/bash

cmake /source "$@"
make -j$(nproc)
