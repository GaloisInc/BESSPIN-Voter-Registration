#!/usr/bin/env bash
set -eo pipefail

ZLIB_PATH="ext/zlib-1.2.11"
SQLITE_PATH="ext/sqlite"  # sqlite downloads with no version number in filename
KCGI_PATH="ext/kcgi-VERSION_0_12_0"
SQLBOX_PATH="ext/sqlbox-VERSION_0_1_12"
ORT_PATH="ext/openradtool-VERSION_0_8_14"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
TARGET_BUILD=${DIR}/../build/host
BVRS_RISCV=${BVRS_RISCV:-0}
BVRS_OS=${BVRS_OS:-linux-gnu}
SQLITE_HOST=""
PRINT_DEFS=${PRINT_DEFS:-0}

if [ "${BVRS_RISCV}" -eq 1 ]; then
    TARGET_BUILD=${DIR}/../build/target
    PREFIX=riscv64-unknown-${BVRS_OS}
    export CC=${PREFIX}-gcc
    export CFLAGS="-I ${TARGET_BUILD}/include"
    export LFLAGS="-L ${TARGET_BUILD}/lib"
    export LD=${PREFIX}-ld
    export AR=${PREFIX}-ar
    # YMMV with the build argument-below
    SQLITE_HOST="--build=x86_64-unknown-linux --host=riscv64-unknown-${BVRS_OS}"
    if [ "${PRINT_DEFS}" -eq 1 ]; then
	    echo "export CC=${CC}"
	    echo "export CFLAGS=${CFLAGS}"
	    echo "export LFLAGS=${LFLAGS}"
	    echo "export LD=${LD}"
	    echo "export AR=${AR}"
	    exit 0
    fi
fi

echo "Building deps in ${TARGET_BUILD}"
export PKG_CONFIG_PATH="${TARGET_BUILD}/lib/pkgconfig"

# 1. Build zlib
if [ -z ${HAVE_ZLIB+x} ]; then
    pushd ${ZLIB_PATH}
    if [ -f "configure.log" ]; then
       make distclean
    fi
    ./configure --prefix=${TARGET_BUILD}
    make && make install
    popd
fi

# 2. Build sqlite
if [ -z ${HAVE_SQLITE+x} ]; then
    pushd ${SQLITE_PATH}
    rm -rf build
    mkdir build
    cd build
    ../configure --disable-readline --disable-editline --disable-tcl --prefix=${TARGET_BUILD} --enable-fts3 ${SQLITE_HOST}
    make && make install
    popd
fi

# 3. Build sqlbox
if [ -z ${HAVE_SQLBOX+x} ]; then
    pushd ${SQLBOX_PATH}
    ./configure PREFIX=${TARGET_BUILD}
    bmake && bmake install
    popd
fi

# 3. Build kcgi
if [ -z ${HAVE_KCGI+x} ]; then
    pushd ${KCGI_PATH}
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

# 4. Build ORT. This is a buildtool, and hence will not
# be compiled with any of the riscv toolchain
if [ -z ${HAVE_ORT+x} ]; then
    pushd ${ORT_PATH}
    if [ -f "Makefile.configure" ]; then
        bmake distclean
    fi
    CC=gcc
    ./configure PREFIX="${DIR}/../build/host"
    bmake && bmake install
    popd
fi
