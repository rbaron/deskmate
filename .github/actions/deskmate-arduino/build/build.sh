#!/bin/bash
set -eux -o pipefail

cd "$GITHUB_WORKSPACE/code/arduino"
cp src/credentials_example.h src/credentials.h
pio run