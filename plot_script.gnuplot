#!/bin/bash
gnuplot << EOF
set terminal png
set output "$2"
set title "Score over time"
set xlabel "Time (seconds)"
set ylabel "Score"
set key on top left
plot for [IDX=0:$3] "$1" i IDX using 1:2 with lines title columnheader(1)
EOF
