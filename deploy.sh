#!/usr/bin/env bash
set -euo pipefail

# This script is really tailored to deploying the built app to a FETT-Target
# repository. We need to know two destination paths, since we use git lfs for
# binary storage: 1. The binary path (e.g.
# SSITH-FETT-Target/SSITH-FETT-Binaries/GFE/appsBinaries/voting/$OS 2. The
# "common" path (e.g. SSITH-FETT-Target/build/voting/common)
#
# The result will be that bvrs/kfcgi get installed in the binary path (1 above)
# and that the empty database + nginx server/fastcgi configuration files are
# installed under path 2 as $common/bvrs.db $common/fastcgi.conf
# $common/sites/voting.conf

build_dir="build/target"

function fail {
    printf '%s\n' "$1" >&2
    exit 1
}

if [ ! $# -eq 2 ]; then
    echo "Usage: deploy.sh <binary_storage_path> <common files path>"
    exit 1
fi

bin_path=$1
common_path=$2

if [ ! -d ${bin_path} ]; then
    fail "${bin_path} does not appear to be a valid directory"
fi

if [ ! -d ${common_path} ]; then
    fail "${common_path} does not appear to be a valid directory"
fi

built_files=("sbin/kfcgi" "bin/bvrs" "share/bvrs.db" "share/voting.conf" "share/fastcgi.conf")

for f in ${built_files[@]}; do
    if [ ! -f "${build_dir}/${f}" ]; then
        fail "${build_dir}/${f} does not appear to have been built, try running 'make dist' in source/src"
    fi
done

install "${build_dir}/sbin/kfcgi" "${bin_path}"
install "${build_dir}/bin/bvrs" "${bin_path}"
install -m 664 "${build_dir}/share/bvrs.db" "${common_path}"
install -m 664 "${build_dir}/share/fastcgi.conf" "${common_path}"
install -m 664 "${build_dir}/share/voting.conf" "${common_path}/conf/sites"
