#!/bin/sh

for i in 0 1 2 3 4 5 6 7
do
	cd client"$i"/;
	./a.out "$@"
	cd ..
done

