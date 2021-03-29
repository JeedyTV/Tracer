#!/bin/bash



gcc test.S -g -o tracee --static -nostdlib -Wl,-entry="main"

echo "compile et run backup.c"
gcc backup.c --static -o out
./out tracee
echo "---compile main.c marche pas"
gcc main.c --static -o out
echo "---run main.c marche pas"
./out tracee