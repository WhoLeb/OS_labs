#!/bin/sh
g++ -c -w lab3.cpp -o lab.o
g++ -o lab.obj lab.o -pthread
./lab.obj
