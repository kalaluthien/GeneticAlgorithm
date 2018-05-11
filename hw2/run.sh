#!/bin/bash

make clean
make
#cp data/weighted_10.txt maxcut.in
#cp data/unweighted_50.txt maxcut.in
#cp data/unweighted_100.txt maxcut.in
#cp data/unweighted_500.txt maxcut.in
#cp data/weighted_500.txt maxcut.in
#cp data/weighted_chimera_297.txt maxcut.in
#cp data/chimera_946.txt maxcut.in
cp data/cubic_1000.txt maxcut.in
#cp data/planar_800.txt maxcut.in
#cp data/random_500.txt maxcut.in
#cp data/random_1000.txt maxcut.in
#cp data/toroidal_800.txt maxcut.in
time ./maxcut maxcut.in maxcut.out
#gprof maxcut gmon.out > anal.txt
