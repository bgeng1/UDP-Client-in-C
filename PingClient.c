/* Ping Client by Bright Geng */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

int main (int argc, char *argv[]) {

  int s,i = 0;
  unsigned short port;
  struct sockaddr_in serverAddress;
  struct hostent *h;
  
  if(argc != 3) {
    printf("usage:%s <host> <port> \n",argv[0]);
    exit(1);
  }

  h = gethostbyname(argv[1]);
  if(h==NULL) {
    printf("%s: unknown host '%s'\n",argv[0],argv[1]);
    exit(1);
  }
  port = htons(atoi(argv[2]));
  
  /* create socket using UDP */
  s = socket(AF_INET, SOCK_DGRAM, 0);
  if(s<0) {
    perror("cannot open socket");
    exit(1);
  }
  //unblock the port
  fcntl(s, F_SETFL, O_NONBLOCK);

  /* set up server details */
  serverAddress.sin_family = h->h_addrtype;
  memcpy((char *) &serverAddress.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
  serverAddress.sin_port = port;
		
 /* sending data to server */
  char msg[5];
  strcpy(msg,"PING");
  socklen_t length;
  length = sizeof(serverAddress);
  double elapsed;
  struct timeval start,end,sent;
  //converts the IP address from to a readable format (from binary to a string)
  char ipAddress[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(serverAddress.sin_addr.s_addr), ipAddress, INET_ADDRSTRLEN);
  
  /* loops through 10 ping requests */
  while(i<10){
    if(sendto(s, msg, (strlen(msg)+1), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress))<0) {
      perror("cannot send data");
      close(s);
      exit(1);
    }
    gettimeofday(&sent,NULL);
    
    //error testing printf
    //printf("%s: sent (%s)\n",argv[0],msg);
    
    int terminate = 1;
    gettimeofday(&start,NULL);
    /* waiting for response from server (receive a reply or exceed 1 second) */
    while(terminate>0){
    	gettimeofday(&end,NULL);
    	if(recvfrom(s, msg, sizeof msg, 0, (struct sockaddr *)&serverAddress, &length)>0){
    		terminate=0;
   		}
   		elapsed = end.tv_sec - start.tv_sec;
   		if(elapsed >= 1){
   		 	terminate=-1;
   		 }
    }
    
    if(terminate==0){
    	printf("Pinging to %s, id: %d, RTT = %d ms \n",ipAddress ,i,(end.tv_usec-sent.tv_usec)/1000);
    }else{
  	    printf("Pinging to %s, id: %d, RTT = Timed Out \n",ipAddress,i);

    }
    i++;
}      
	close(s);
   

return 0;
  
}

