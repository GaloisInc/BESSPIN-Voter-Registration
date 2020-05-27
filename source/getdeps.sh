#!/usr/bin/env bash
set -euo pipefail

SQLITE_VERSION=3.22.0

ZLIB_URL="https://www.zlib.net/zlib-1.2.11.tar.gz"
SQLITE_URL="https://www.sqlite.org/src/tarball/sqlite.tar.gz"
KCGI_URL="https://github.com/kristapsdz/kcgi/archive/VERSION_0_12_0.tar.gz"
SQLBOX_URL="https://kristaps.bsd.lv/sqlbox/snapshots/sqlbox.tar.gz"
ORT_URL="https://github.com/kristapsdz/openradtool/archive/VERSION_0_8_13.tar.gz"
### Fetch sources ###

echo "****"
echo "Fetching zlib"
echo "****"
curl -L -G ${ZLIB_URL} -o ext/zlib.tar.gz
if [ $? -ne 0 ]; then
    echo "Error fetching zlib sources"
    exit 1
fi

echo "****"
echo "Fetching sqlite"
echo "****"
curl -L -G -d "r=version-${SQLITE_VERSION}" ${SQLITE_URL} --output ext/sqlite.tar.gz
if [ $? -ne 0 ]; then
    echo "Error fetching sqlite sources"
    exit 1
fi

echo "****"
echo "Fetching sqlbox"
echo "****"
curl -L -G ${SQLBOX_URL} -o ext/sqlbox.tar.gz
if [ $? -ne 0 ]; then
    echo "Error fetching sqlbox sources"
    exit 1
fi

echo "****"
echo "Fetching kcgi"
echo "****"
curl -L -G ${KCGI_URL} -o ext/kcgi.tar.gz
if [ $? -ne 0 ]; then
    echo "Error fetching kcgi sources"
    exit 1
fi

echo "****"
echo "Fetching openradtool"
echo "****"
curl -L -G ${ORT_URL} -o ext/ort.tar.gz
if [ $? -ne 0 ]; then
    echo "Error fetching kcgi sources"
    exit 1
fi

echo "****"
echo "Done fetching sources, unpacking"
echo "****"

cd ext/
tar -xzvf zlib.tar.gz
if [ $? -ne 0 ]; then
    echo "Error extracting zlib"
    exit 1
fi

tar -xzvf sqlite.tar.gz
if [ $? -ne 0 ]; then
    echo "Error extracting sqlite"
    exit 1
fi

tar -xzvf sqlbox.tar.gz
if [ $? -ne 0 ]; then
    echo "Error extracting sqlbox"
    exit 1
fi

tar -xzvf kcgi.tar.gz
if [ $? -ne 0 ]; then
    echo "Error extracting kcgi"
    exit 1
fi

tar -xzvf ort.tar.gz
if [ $? -ne 0 ]; then
    echo "Error extracting openradtool"
    exit 1
fi
