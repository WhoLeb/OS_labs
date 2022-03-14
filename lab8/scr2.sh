#!/bin/sh
g++ -c -w sec.cpp -o lab2.o
g++ -o lab2.obj lab2.o -pthread -lrt
./lab2.obj
read line
rm lab2.o
rm lab2.obj
