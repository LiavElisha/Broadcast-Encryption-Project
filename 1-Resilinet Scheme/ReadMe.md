This is the 1-resilinet scheme - 
The server define Ki for every user. Then, the server gives every client i all keys except Ki.
The server encrypts the message by XOR the messgae with all Ki (such that i is a user which doesn't have permission see the message).
Finally, the server transmits an share encrypted message to all members.

I ceated a few scripts in order to make it simple to use - 
compileClients.sh - Compile all client.c files.
deleteFiles.sh - Delete all files in /client{index}/files/*
runClients.sh {Resilient clients} - Run all clients with the input - {Number of files}. For example - runClients.sh 0 2 4
runServer.sh {Resilient clients} - Run the server with the input - {Resilient clients}. For example - runClients.sh 0 2 4
diff.sh - for every client runs diff command between client plaintext file and server plaintext file

