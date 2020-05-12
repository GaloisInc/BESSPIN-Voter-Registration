#!/usr/bin/env bash
set -eo pipefail

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
TARGET_BUILD=${DIR}/../build/host
BVRS_RISCV=${BVRS_RISCV:-0}
BVRS_OS=${BVRS_OS:-linux}
SQLITE_HOST=""

if [ "${BVRS_RISCV}" -eq 1 ]; then
    TARGET_BUILD=${DIR}/../build/target
    PREFIX=riscv64-unknown-${BVRS_OS}-gnu
    export CC=${PREFIX}-gcc
    export CFLAGS="-I ${TARGET_BUILD}/include"
    export LFLAGS="-L ${TARGET_BUILD}/lib"
    export LD=${PREFIX}-ld
    export AR=${PREFIX}-ar
    SQLITE_HOST="--host=riscv64-unknown-${BVRS_OS}"
fi

echo "Building deps in ${TARGET_BUILD}"
export PKG_CONFIG_PATH="${TARGET_BUILD}/lib/pkgconfig"

# 1. Build zlib
if [ -z ${HAVE_ZLIB+x} ]; then
    pushd ext/zlib-1.2.11
    if [ -f "configure.log" ]; then
       make distclean
    fi
    ./configure --prefix=${TARGET_BUILD}
    make && make install
    popd
fi

# 2. Build sqlite
if [ -z ${HAVE_SQLITE+x} ]; then
    pushd ext/sqlite
    rm -rf build
    mkdir build
    cd build
    ../configure --disable-tcl --prefix=${TARGET_BUILD} --enable-fts3 ${SQLITE_HOST}
    make && make install
    popd
fi

# 3. Build sqlbox
if [ -z ${HAVE_SQLBOX+x} ]; then
    pushd ext/sqlbox-0.1.12
    ./configure PREFIX=${TARGET_BUILD}
    bmake && bmake install
    popd
fi

# 3. Build kcgi
if [ -z ${HAVE_KCGI+x} ]; then
    pushd ext/kcgi-VERSION_0_12_0
    if [ -f "Makefile.configure" ]; then
        bmake distclean
    fi
    echo "HAVE_SECCOMP_FILTER=0" > configure.local
    echo "HAVE_GETPROGNAME=0" >> configure.local
    export CFLAGS="${CFLAGS} `pkg-config --cflags zlib` `pkg-config --libs zlib --static`"
    ./configure PREFIX=${TARGET_BUILD}
    bmake && bmake install
    popd
fi
