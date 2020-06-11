#!/usr/bin/env bash
set -euo pipefail

SQLITE_VERSION=3.22.0

zlib_URL="https://www.zlib.net/zlib-1.2.11.tar.gz"
sqlite_URL="https://www.sqlite.org/src/tarball/sqlite.tar.gz?r=version-${SQLITE_VERSION}"
kcgi_URL="https://github.com/kristapsdz/kcgi/archive/VERSION_0_12_0.tar.gz"
sqlbox_URL="https://github.com/kristapsdz/sqlbox/archive/VERSION_0_1_12.tar.gz"
openradtool_URL="https://github.com/kristapsdz/openradtool/archive/VERSION_0_8_14.tar.gz"

banner() {
  echo "****"
  echo "$1"
  echo "****"
}

cmd_or() {
  msg=$1
  shift
  if ! "$@"; then echo "$msg" > /dev/stderr && exit 1; fi
}

### Create "ext" directory if it doesn't exist

banner "Ensuring 'ext' directory exists"
mkdir -p ext

### Fetch sources ###

for tool in zlib sqlite sqlbox kcgi openradtool; do
  banner "Fetching $tool"
  url="${tool}_URL"
  cmd_or "Error fetching ${tool} sources" curl -L -G "${!url}" -o ext/$tool.tar.gz
done

banner "Done fetching sources, unpacking"

cd ext/
for tool in zlib sqlite sqlbox kcgi openradtool; do
  cmd_or "Error extracting ${tool}" tar -xzvf $tool.tar.gz
done
