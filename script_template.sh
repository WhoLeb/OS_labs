#!/bin/sh
g++ -c lab.cpp
g++ -o lab.obj lab.o -pthread
./lab.obj
