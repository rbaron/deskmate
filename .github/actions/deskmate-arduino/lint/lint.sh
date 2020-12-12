#!/bin/ash
set -eux -o pipefail

cd "$GITHUB_WORKSPACE/code/arduino"
make lint