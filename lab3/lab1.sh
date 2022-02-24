#!/bin/sh
g++ -c lab1.cpp
g++ -o lab.obj lab1.o -pthread
./lab.obj
