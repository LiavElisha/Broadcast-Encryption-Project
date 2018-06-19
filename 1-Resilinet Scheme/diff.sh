#!/bin/sh

for i in 0 1 2 3 4 5 6 7 8 9
do
	echo "diff with original file, client $i"
	diff ./client"$i"/files/file.txt ./server/dataFiles/fileToEncrypt.txt
done

