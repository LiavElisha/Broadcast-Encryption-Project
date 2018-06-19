This is the naive scheme - 
Give every user its own key and transmit an individually encrypted message to every member of the privileged class.

I wrote a few scripts which run the program.
compileClients.sh - Compile all client.c files.
deleteFiles.sh - Delete all files in /client{index}/files/*
runClients.sh {Resilient clients} - Run all clients with the input - {Resilient clients}. For example - runClients.sh 0 1 2 5 7
runServer.sh {Resilient clients} - Run the server with the input - {Resilient clients}. For example - runClients.sh 0 1 2 5 7

