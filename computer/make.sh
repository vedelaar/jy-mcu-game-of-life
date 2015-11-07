#!/bin/bash
cp ../bare_code/gameoflife.* .
gcc main.cpp gameoflife.cpp -std=c++11
