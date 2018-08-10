# UDP-Client-in-C
A client to ping a server using UDP in c (my first experience with socket programming!)

Specifications:
 - Sends 10 messages (pings) to a server which will send the exact same message back (echo)(server was sourced from someone else, used it in order to test client)
 - The client should wait 1 second for a reply, if the time exceeds one second then assume that the packet was dropped
 - Output the result of the ping to standard output with 3 fields: the server address, ping number, round trip time (rtt) e.g. "Ping to 127.0.0.1, seq = 1, rtt: 120ms"
 - usage: $PingClient \<host\> \<port\> 
 
  where host = IP of the system running the client
        port = port number to use for communication
 - must use UDP and not TCP
