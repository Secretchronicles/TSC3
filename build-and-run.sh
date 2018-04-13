#!/bin/bash

# Get latest git submodules
git submodule init
git submodule update

# Delete old build files
rm -rf build
mkdir build
cd build

# Delete old game directory at %HOME/tsc3
rm -rf ~/tsc3

# Build TSC3
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=~/tsc3 ..
make

# Install TSC to $HOME/tsc3
make install

# Changing to game directory
cd ~/tsc3

# Start TSC3
./bin/tsc
