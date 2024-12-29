#!/bin/sh

set -e
cd /crun

git config --global --add safe.directory /crun

git clean -fdx
./autogen.sh
./configure CFLAGS='-Wall -Wextra -Werror'
make -j "$(nproc)"

git clean -fdx
./autogen.sh
./configure CFLAGS='-Wall -Wextra -Werror' --disable-systemd
make -j "$(nproc)"
