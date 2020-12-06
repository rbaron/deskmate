#!/bin/ash
set -eux -o pipefail

cd "$GITHUB_WORKSPACE/code/lib/deskmate"
make lint