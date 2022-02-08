#!/bin/sh
g++ -c sem1.cpp -o sem.o
g++ -o lab.obj sem.o -pthread
./lab.obj
