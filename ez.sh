#!/bin/bash

clear
sudo make clean uninstall
make test DEBUG=1 VALGRIND=1
sudo make install
