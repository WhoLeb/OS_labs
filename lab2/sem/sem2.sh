#!/bin/sh
g++ -c sem2.cpp -o sem.o
g++ -o lab.obj sem.o -pthread
./lab.obj
