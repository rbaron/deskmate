#!/bin/ash
set -eux -o pipefail

cd "$GITHUB_WORKSPACE/code/lib/deskmate"
find src -iname *.h -o -iname *.cc | xargs clang-format -n -Werror