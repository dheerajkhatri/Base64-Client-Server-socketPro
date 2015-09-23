# Base64 Client Server Communication System #
* Server can handle multiple clients simultaneously and it uses Base64 encoding and both client server works on TCP sockets.
* Initially server will be waiting for TCP connection from client.
* Then Client will connect to server using server's TCP port that is already known to the client.
* After Successful connection,client will receive input msg from user and encodes it using Base64 encoding.
* Once encoded message is computed the client sends the message to the server via TCP port.
* After receving the message server prints original msg and decoded msg and then sends ACK to client.
* Client and server remain in loop to send any number of messages.
* Once Client wants to close communication,it sends msg to server and the TCP connections on both closed gracefully by releasing socket resources.


### How to run the Program:- ###
* Start the server it will take one command line argument as port number on which it will work
* Start the client with 2 command line arguments first is server ip address and second is server port
* You can run both client and server on same computer as they 

Example:</br>
server  : sudo ./server 5000</br>
client1 : sudo ./client 127.0.0.1 5000</br>
client2 : sudo ./client 127.0.0.1 5000</br>

Client:- executableCode ServerIP ServerPort</br>
Server:- executableCode ServerPort</br>

### Further Reading and References ###
* http://www.cs.dartmouth.edu/~campbell/cs50/socketprogramming.html
