#!/bin/sh
g++ -c -w mut1.cpp -o mut.o
g++ -o lab.obj mut.o -pthread
./lab.obj
