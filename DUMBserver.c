#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int serverRunning = 1;

int main(int argc, char * argv[]){
 /*
 if(argc < 2){
    printf("There are not enough arguments\n");
  }
  char * ip = argv[1];
  int port = atoi(argv[2]);
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
  
  
   

     do{
      memset(buffer,0, sizeof(buffer)); 
      recv(client_sock,buffer,sizeof(buffer),0); 
      char * token;
      char * list[3];
      int i = 0;
      if(strchr(buffer, ' ') != NULL){
        token = strtok(temp, " ");
        //Split command into tokens
        while(token != NULL ) {
          printf("Inside tokenizer\n");
          list[i] = token;
          token = strtok(NULL, " ");
          i++;
        }       
      }
        
      printf("%s\n", buffer);
      printf("Buffer length = %d\n", strlen(buffer));
      int test = strcmp(buffer, "HELLO");
      printf("It returns %d\n", test);
      printf("I is %d\n",i);
      
      if(i == 0){
        if(strcmp(buffer,"HELLO") == 0){
        printf("Here\n");
        write(client_sock, "HELLO DUMBv0 ready!", 19);
        }else if(strcmp(buffer,"GDBYE") == 0){
          serverRunning = 0;
        }else{
          write(client_sock, "ER:EMPTY", 19);  
        }
      }else if(i == 1){
        if(strcmp(list[0],"CREAT") == 0){
           if(strlen(list[1]) < 5 || strlen(list[1]) > 25 || !(isalpha(list[1][0]))){
              write(client_sock, "ER:WHAT?", 8); 
           }else{
             //ADD IMPLEMENTATION FOR MESSAGE BOXES 
             printf("Creates a message box\n");
           }
        }
      }else if(i == 2){
        
      }else{
        write(client_sock, "ER:WHAT?", 8); 
      }
      
      
    	memset(buffer,0, sizeof(buffer));
     }while(serverRunning);

  close(client_sock);
  close(server_sock);
  return 0;
 

}