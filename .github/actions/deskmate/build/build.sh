#!/bin/ash
set -eux -o pipefail

cd "$GITHUB_WORKSPACE/code/lib/deskmate"
rm -rf build
mkdir build
cd build
cmake ..
make