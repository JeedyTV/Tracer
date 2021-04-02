#!/bin/bash

rm -f src.tar.gz
mkdir src
cp Makefile src/Makefile
cp report.pdf src/report.pdf
for fichier in *.c
do
cp $fichier src/$fichier
done
for fichier in *.h
do
cp $fichier src/$fichier
done
tar czvf src.tar.gz src
rm -rf src