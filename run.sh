#!/bin/bash

gcc test.c --static -o out
gcc test.S -g -o tracee --static -nostdlib -Wl,-entry="main"
./out tracee
