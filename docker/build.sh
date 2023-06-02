#!/bin/sh
set -e

SCRIPTDIR=$(realpath $(dirname $0))
BASEDIR=$(realpath ${SCRIPTDIR}/../)
BUILDDIR="${BASEDIR}/build/"

mkdir -p ${BUILDDIR}
cd ${BUILDDIR}

cmake ${BASEDIR} -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel $(nproc)
