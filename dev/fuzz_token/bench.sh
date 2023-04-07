#!/bin/bash

gcc bench.c -o bench -O3 -lparsher && ./bench
