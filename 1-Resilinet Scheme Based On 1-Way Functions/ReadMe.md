This is the 1-resilinet scheme based on 1-way functions - 
The scheme is identical to the last scheme except the using 1-way functions.
By this functions clients can store O(log n) keys rathe than n+1 if the keys are pseudo-randomly generated from a common seed
where the pseudo-random function f output is twice the length of the input.
More details on Amos Fiat' and Moni Naor' paper. 

I created a few scripts in order to make it simple to use - 
compileClients.sh - Compile all client.c files.
deleteFiles.sh - Delete all files in /client{index}/files/*
runClients.sh {Resilient clients} - Run all clients with the input - {Number of files}. For example - runClients.sh 0 2 4
runServer.sh {Resilient clients} - Run the server with the input - {Resilient clients}. For example - runClients.sh 0 2 4
diff.sh - for every client runs diff command between client plaintext file and server plaintext file

