#!/bin/bash
#compile
/usr/bin/g++  /home/cip/sri-workspace/Source.cpp -o /home/cip/sri-workspace/Source -I /usr/include/mysql -L /usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -latomic -ldl -lpcap -Wall
#execute
/home/cip/sri-workspace/Source
