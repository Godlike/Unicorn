#!/bin/sh
set -e

mkdir $BUILD_DIR
cd $BUILD_DIR
cmake -DCMAKE_CXX_COMPILER=$COMPILER ..

if [ "$BUILD_DOCUMENTATION" != "true" ]; then
    make -j${nproc}
    ctest -VV
else
    make docs
fi
