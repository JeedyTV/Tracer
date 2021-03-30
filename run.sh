#!/bin/bash



gcc test.S -g -o tracee --static -nostdlib -Wl,-entry="main"

echo "---compile fun_tree et main.c marche"
gcc fun_tree.c main.c --static -o out
echo "---run main.c marche"
./out tracee
