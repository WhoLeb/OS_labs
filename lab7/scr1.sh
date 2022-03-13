#!/bin/sh
g++ -c fir.cpp -o lab.o
g++ -o lab.obj lab.o -pthread -lrt
./lab.obj
read line
