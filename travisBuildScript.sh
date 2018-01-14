#!/bin/sh
set -e

mkdir $BUILD_DIR
cd $BUILD_DIR

CMAKE_FLAGS="-DCMAKE_CXX_COMPILER=$COMPILER"

if [ "$BUILD_DOCUMENTATION" = "true" ]; then
    CMAKE_FLAGS="$CMAKE_FLAGS -DUNICORN_BUILD_DOCUMENTATION=ON"
fi

cmake $CMAKE_FLAGS ..

if [ "$BUILD_DOCUMENTATION" != "true" ]; then
    make -j2
    ctest -VV
else
    make docs
fi
