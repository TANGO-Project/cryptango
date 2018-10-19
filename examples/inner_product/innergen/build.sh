#!/bin/bash
LIBS="-L$HOME/.local/lib -L/usr/lib -L/usr/lib/x86_64-linux-gnu -lcryptmr -lntl -lgmp -lgf2x -ljsoncpp -lcryptopp -lpthread"
CXXFLAGS="-L$HOME/.local/include -std=c++11"
compss_build_app --LIBS="$LIBS" --CXXFLAGS="$CXXFLAGS" innergen
