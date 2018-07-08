#!/bin/sh
set -e

mkdir $BUILD_DIR
cd $BUILD_DIR

CMAKE_FLAGS="-DCMAKE_CXX_COMPILER=$COMPILER"

if [ -n "$CMAKE_BUILD_TYPE" ]; then
    CMAKE_FLAGS="$CMAKE_FLAGS -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE"
fi

if [ -n "$COMPILER_FLAGS" ]; then
    CMAKE_FLAGS="$CMAKE_FLAGS -DCMAKE_CXX_FLAGS=$COMPILER_FLAGS"
fi

if [ "$BUILD_DOCUMENTATION" = "true" ]; then
    CMAKE_FLAGS="$CMAKE_FLAGS -DUNICORN_BUILD_DOCUMENTATION=ON"
fi

cmake $CMAKE_FLAGS ..

if [ "$BUILD_DOCUMENTATION" != "true" ]; then
    make -j2 # Assimp with lot of cores eats too much RAM and build fails
    ctest -VV
else
    make docs
fi
