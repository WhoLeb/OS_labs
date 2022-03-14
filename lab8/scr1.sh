#!/bin/sh
g++ -w -c fir.cpp -o lab.o
g++ -w -o lab.obj lab.o -pthread -lrt
./lab.obj
rm lab.o
rm lab.obj
