#!/bin/bash
#compile
/usr/bin/g++  /home/cip/sri-workspace/bkp/Source_bkp.cpp -o /home/cip/sri-workspace/bkp/Source_bkp -I /usr/include/mysql -L /usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -latomic -ldl -lpcap -Wall
#execute
/home/cip/sri-workspace/bkp/Source_bkp
