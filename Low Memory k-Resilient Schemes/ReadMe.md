This is the Low Memory k-Resilient Schemes  - 
You can find details about this scheme on Amos Fiat' and Moni Naor' paper & on the poster.

I created a few scripts in order to make it simple to use - 
compileClients.sh - Compile all client.c files.
deleteFiles.sh - Delete all files in /client{index}/files/*
runClients.sh {Resilient clients} - Run all clients with the input - {Number of files}. For example - runClients.sh 0 2 4
runServer.sh {Resilient clients} - Run the server with the input - {Resilient clients}. For example - runClients.sh 0 2 4
diff.sh - for every client runs diff command between client plaintext file and server plaintext file

