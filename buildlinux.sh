#!/bin/bash

echo "First Arg: $1";

ARG1=${1:-0}
ARG2=${2:-0}
ARG3=${3:-0}

RELEASE_MODE=0
REGEN=0
RUN=0

if [ $ARG1 == "release" ]; then RELEASE_MODE=1; fi
if [ $ARG2 == "release" ]; then RELEASE_MODE=1; fi
if [ $ARG3 == "release" ]; then RELEASE_MODE=1; fi

if [ $ARG1 == "run" ]; then RUN=1; fi
if [ $ARG2 == "run" ]; then RUN=1; fi
if [ $ARG3 == "run" ]; then RUN=1; fi

if [ $ARG1 == "regen" ]; then REGEN=1; fi
if [ $ARG2 == "regen" ]; then REGEN=1; fi
if [ $ARG3 == "regen" ]; then REGEN=1; fi


if [ $REGEN == 1 ]; then
    rm -rf build/;
    mkdir build;
fi

cd build

if [ $RELEASE_MODE == 1 ]; then
   cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..
fi

if [ $RELEASE_MODE == 0 ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug -GNinja ..
fi

ninja

if [ $RUN == 1 ]; then
    cd Sandbox
    ./Sandbox
fi
