#!/bin/sh
rm output_file
gnome-terminal -- ./scr2.sh
g++ -c fir.cpp -o lab.o
g++ -o lab.obj lab.o -pthread
./lab.obj

