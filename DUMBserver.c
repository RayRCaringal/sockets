#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int serverRunning = 1;
int instanceIntiated = 0;

int main(int argc, char * argv[]){
  /*
 if(argc < 2){
    printf("There are not enough arguments\n");
  }
  char * ip = argv[1];
  int port = atoi(argv[2]);
  struct hostent *host = gethostbyname(ip);
  if(host == NULL){
    printf("ERRROR, no such host\n");
    return;
  }
  */
  //Create Socket
  int server_sock = socket(AF_INET, SOCK_STREAM,0);
  int z = 1;
    char buffer[40];
    char temp[40];
    char * str;
  setsockopt(server_sock, SOL_SOCKET, SO_LINGER,&z,sizeof(z)); // actually kills the socket when it is cloed. 
	setsockopt(server_sock,SOL_SOCKET,SO_REUSEADDR,&z,sizeof(z));
  
  
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET; // IPv4
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set IP address
  server_addr.sin_port = htons(1234); // set port
  bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)); // Bind
  
  listen(server_sock, 20);
  
  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  int client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size); 
  // recv(client_sock,buffer,sizeof(buffer),0);
  
  
   
  //  printf("Buffer length = %d\n", strlen(buffer));
   // int test = strcmp(buffer, "HELLO");
   // printf("It returns %d\n", test);
     do{
      recv(client_sock,buffer,sizeof(buffer),0);   
      printf("%s\n", buffer);
      if(strcmp(buffer,"HELLO") == 0){
        write(client_sock, "HELLO DUMBv0 ready!", 19);
        instanceIntiated = 1;
      }else{
        write(client_sock, "Invalid", 7);
      }

      
      if(strcmp(buffer,"GDBYE") == 0)serverRunning = 0;
      
    	memset(buffer,0, sizeof(buffer));
     }while(serverRunning);
    

 /*do{
        if((strcmp(temp,"GDBYE") == 0))serverRunning = 0;

      }while(serverRunning);
    char str[] = "Exiting!";
    write(client_sock, str, sizeof(str));
    }
    */

  close(client_sock);
  close(server_sock);
  return 0;
 

}