#!/bin/sh
g++ -c -w lab2.cpp -o lab.o
g++ -o lab.obj lab.o -pthread
./lab.obj
