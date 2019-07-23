#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

sudo chown -R $USER: $SCRIPT_DIR/build
sudo chown -R $USER: $SCRIPT_DIR/install
