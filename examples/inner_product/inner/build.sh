#!/bin/bash
LIBS="-L$HOME/.local/lib -L/usr/lib -L/usr/lib/x86_64-linux-gnu -lntl -lgmp -lgf2x -ljsoncpp -L/usr/local/cuda/lib64 -lxmp -lcudart -lpthread"
CXXFLAGS="-I$HOME/.local/include -std=c++11"
compss_build_app --LIBS="$LIBS" --CXXFLAGS="$CXXFLAGS" inner
