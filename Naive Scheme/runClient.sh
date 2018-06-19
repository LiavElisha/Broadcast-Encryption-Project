#!/bin/sh

for i in 0 1 2 3 4 5 6 7 8 9
do
	cd client"$i"/;
	./a.out "$@"
	
	# ENCRYPTED_FILE_SIZE=$(ls -l files/sample_file.enc | awk '{print $5}')
	# X=$(($ENCRYPTED_FILE_SIZE%256))
	# if [ $X -eq 0 ]
	# then
	# 	echo "The encrypted file size is product of 256, resize the fileToEncrypt file"
	# fi
	cd ..
done

