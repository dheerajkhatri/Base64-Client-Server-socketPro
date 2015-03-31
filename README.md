# Base64-Client-Server-socketPro

Application #5: Base64 encoding system using Client-Server socket programming

In this assignment, you require to implement two C programs, namely server and client to
communicate with each other based on TCP sockets. The aim is to implement simple Base64
encoding communication protocol.

Initially, server will be waiting for a TCP connection from the client. Then, client will connect to
the server using server’s TCP port already known to the client. After successful connection, the
client accepts the text input from the user and encodes the input using Base64 encoding system.
Once encoded message is computed the client sends the Message (Type 1 message) to the server
via TCP port. After receiving the Message, server should print the received and original message by
decoding the received message, and sends an ACK to the client. The client and server should
remain in a loop to communicate any number of messages. Once the client wants to close the
communication, it should send a Message (Type 2 Message) to the server and the TCP connection
on both the server and client should be closed gracefully by releasing the socket resource.

The messages used to communicate contain the following fields:
Message_
Messag
Type
e
1. Message_type : integer
2. Message
: Character [MSG_LEN], where MSG_LEN is an integer constant
3. < Message> content of the message in Type 2can be anything.
You also require implementing a "Concurrent Server", i.e., a server that accepts connections from
multiple clients and serves all of them concurrently.
You should accept the IP Address and Port number from the command line (Don't use a hard-coded
port number). Prototype for command line is as follows:
Prototypes for Client and Server
Client :<executable code><Server IP Address><Server Port number>
Server :<executable code><Server Port number>
*Please make necessary and valid assumptions whenever required.


Base64 Encoding System Description:

Base64 encoding is used for sending a binary message over the net. In this scheme, groups of 24bit
are broken into four 6 bit groups and each group is encoded with an ASCII character. For binary
values 0 to 25 ASCII character ‘A’ to ‘Z’ are used followed by lower case letters and the digits for
binary values 26 to 51 & 52 to 61 respectively. Character ‘+’ and ‘/’ are used for binary value 62 &
63 respectively. In case the last group contains only 8 & 16 bits, then “==” & “=” sequence are
appended to the end.
