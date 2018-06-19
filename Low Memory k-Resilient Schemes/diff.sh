#!/bin/sh

for i in 0 1 2 3 4 5 6 7 8 9
do
	echo "diff with 1-file, client $i"
	diff ./client"$i"/files/1.txt ./server/dataFiles/fileToEncrypt1.txt
done

