#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define _GNU_SOURCE
//"127.0.0.1" IP
//1234 Port 
int serverIsLive = 1;
int instanceIntiaited = 0;
/*
void * listener(void * sock){
  int * sock_ptr = (int *) sock;
  char buffer[40]; 
  while(strcmp(buffer, "disconnected from server\n" != 0)){
    int temp = read(*sock_ptr, buffer, 40);
    if(temp == 0){ //There are no bytes 
        printf("Exiting server");
        serverIsLive =0;
        exit(0);
    }else{
    printf("Received %s\n", buffer);
    }
    
  if(strcmp(buffer, "disconnected from server\n" == 0)){
    serverIsLive =0;
    break;
  }
  memset(buffer,0, sizeof(buffer));	
 }
   exit(0);
}  
  */


/*
      quit			(which causes: E.1 GDBYE)
			create		(which causes: E.2 CREAT)
			delete		(which causes: E.6 DELBX)			
			open		(which causes: E.3 OPNBX)
			close		(which causes: E.7 CLSBX)
			next			(which causes: E.4 NXTMG)
			put			(which causes: E.5 PUTMG)
      */

const char * commands[] = {
    "create",
    "delete",
    "open",
    "close",
    "next",
    "put",
};

int isCommand(char * buffer){
  int i;
  int size = (sizeof(commands))/(sizeof(const char *));
  for(i = 0; i < size; i++){
   if(strcmp(buffer,commands[i]) == 0){
     return 1;
   } 
  }
  return 0;
}


int main(int argc, char * argv[]){
 
  if(argc < 2){
    printf("There are not enough arguments\n");
  }
  
 char * ip = argv[1];
 int port = atoi(argv[2]);
 char command [40];
 char buffer[40];
 /* struct hostent *host = gethostbyname(ip);
  if(host == NULL){
    printf("ERRROR, no such host\n");
    return;
  }*/
  
  
  //Create a socket IPv4, TCP
  int sock = socket(AF_INET, SOCK_STREAM,0); //Client's Socket
  struct sockaddr_in server_addr;
  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET; //IPv4
  server_addr.sin_addr.s_addr = inet_addr(ip); // IP address 
  server_addr.sin_port = htons(port); //Port 
 // connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
  int connection_counter = 0;
  //Atempt to connect 3 times
  while(connect(sock,(struct sockaddr *) &server_addr,sizeof(server_addr)) < 0){
    if(connection_counter == 3){
      printf("Failed to connect...shutting down\n");
      return;
    }
		printf("Error connecting, retrying...\n");
    connection_counter++;
		sleep(2);
	}
  
  //Checks for HELLO or help command or quit command 
  do{
    scanf("%s", command);
    if(isCommand(command)){ //It is not Hello or Quit or Help
       if(instanceIntiaited){
         //Other commands 
      
        }else{
         printf("A session with DUMB server was not intialized, please use the command HELLO to start.\n");
       }
    }else if(strcmp(command,"HELLO") == 0){
      char str[40] = {"HELLO"};
       send(sock,str,sizeof(str),0);
       read(sock,buffer,40);
        if(strcmp(buffer,"HELLO DUMBv0 ready!") == 0){
          if(instanceIntiaited){
            printf("An instance is already initalized, if you would like to close this instance please use the command 'quit'.\n");
          }else{
            printf("%s \n", buffer);
            instanceIntiaited = 1;
          }
      }
    }else if(strcmp(command, "quit") == 0){
      send(sock,"GDBYE",5,0);
      serverIsLive = 0;
    }else if(strcmp(command,"help") == 0) {
      printf("Commands are: \n hello \n quit \n create \n delete \n open\n close\n next\n put\n");  
    }else{
      printf("This is an invalid command, please type 'help' for a list of commands\n");
    }
	memset(buffer,0, sizeof(buffer));
 }while(serverIsLive);
  
  
  //Read from server  
 // char buffer[40];
  //read(sock,buffer,sizeof(buffer)-1);
 
  //Close 
  close(sock);
  return 0;
  
  
  
  
}