#!/bin/sh
g++ -c sec.cpp -o lab2.o
g++ -o lab2.obj lab2.o -pthread
./lab2.obj
read line
