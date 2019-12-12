#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "DUMB.h"
#define _GNU_SOURCE
//"127.0.0.1" IP
//8080 Port
int serverIsLive = 1;
int instanceIntiaited = 0;

/*
      quit			(which causes: E.1 GDBYE)
			create		(which causes: E.2 CREAT)
			delete		(which causes: E.6 DELBX)			
			open		(which causes: E.3 OPNBX)
			close		(which causes: E.7 CLSBX)
			next			(which causes: E.4 NXTMG)
			put			(which causes: E.5 PUTMG)
      */

const char *commands[] = {
    "create",
    "delete",
    "open",
    "close",
    "next",
    "put",
};

int isCommand(char *buffer){
    int i;
    int size = (sizeof(commands)) / (sizeof(const char *));
    for (i = 0; i < size; i++){
        if (strcmp(buffer, commands[i]) == 0)return 1;
    }
    return 0;
}


//Check for port fails to bind <- Not Yet Implemented
int main(int argc, char *argv[]){
    /*
  if(argc < 2){
    printf("There are not enough arguments\n");
  }
  
 char * ip = argv[1];
 int port = atoi(argv[2]);
 */
    char command[40];
    char buffer[40];
    
    //Create a socket IPv4, TCP
    int sock = socket(AF_INET, SOCK_STREAM, 0); //Client's Socket
    struct sockaddr_in server_addr;
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;                     //IPv4
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP address
    server_addr.sin_port = htons(1234);                   //Port
                                                          // connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    int connection_counter = 0;
    //Atempt to connect 3 times
    while (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        if (connection_counter == 3){
            printf("Failed to connect...shutting down\n");
            return;
        }
        printf("Error connecting, retrying...\n");
        connection_counter++;
        sleep(2);
    }

    //Checks for HELLO or help command or quit command
    
    do{
        memset(command,0,sizeof(command));
        memset(buffer,0,sizeof(buffer));
        fgets(command, 40, stdin);
        char *list[5];
        int i = 0;
        int total = strlen(command);
        printf("Command is %s\n", command);
        list[i] = strtok(command, " ");
        char * s2;
        while(list[i] != NULL){
          i++;
          if(i == 2){
          int len = strlen(list[0]) + strlen(list[1])+2;
          int len2 = total - len;
          if(len2 < 0) break; //You're on the 2nd arguments
          s2 = malloc(len2+1); // one for the null terminator
          memcpy(s2, (command+len), len2);
          s2[len2] = '\0';
          list[i++] = s2;
          break;
        }
        list[i] = strtok(NULL, " ");
  }
     
        printf("I is %d\n", i);
        printf("List[0] = %s\n", list[0]);
        if (isCommand(list[0])){
            if (instanceIntiaited){
            //CREATE
               // printf("List[1] = %s\n", list[1]);
               // printf("List = %d\n", strcmp(list[0], "create"));
                if (strcmp(list[0], "create") == 0){
                        int created = 1;
                        do{
                            memset(buffer, 0, sizeof(buffer));
                            char str[] = "CREAT ";
                            if (i == 1){
                                printf("create:>\n");
                                scanf("%s", command);
                                strcat(str, command);
                            }else if (i == 2){
                                strcat(str, list[1]);
                                i--;
                            }
                            send(sock, str, strlen(str), 0);
                            read(sock, buffer, 40);
                            if (strcmp(buffer, "Ok!") == 0){
                                printf("%s\n", buffer);
                                created = 0;
                            }else{
                               printf("%s\n", buffer);
                            }
                        }while (created);
                }
              
              //OPEN
              else if(strcmp(list[0], "open") == 0){
                     int opened = 1;
                     do{
                        memset(buffer, 0, sizeof(buffer));
                        char str[] = "OPNBX ";
                       if(i == 1){
                         printf("open:>\n");
                         scanf("%s", command);
                         strcat(str,command);
                       }else if(i == 2){
                         strcat(str,list[1]);
                         i--;
                       }
                       send(sock, str, strlen(str), 0);
                       read(sock, buffer, 40);
                      if (strcmp(buffer, "Ok!") == 0){
                          printf("\n%s\n", buffer);
                          opened = 0;
                      } 
                     }while(opened);
                }
              
              //CLOSE 
              else if(strcmp(list[0], "close") == 0){
                       char str[] = "CLSBX ";
                       if(i == 1){
                         printf("close:>\n");
                         scanf("%s", command);
                         strcat(str,command);
                       }else if(i == 2){
                         strcat(str,list[1]);
                         i--;
                       }
                       send(sock, str, strlen(str), 0);
                       read(sock, buffer, 40);
                       if (strcmp(buffer, "Ok!") == 0) printf("\n%s\n", buffer);
                      }
                
                //DELETE
                else if(strcmp(list[0], "delete") == 0){
                  char str[] = "DELBX ";
                  if(i == 1){
                    printf("close:>\n");
                    scanf("%s", command);
                    strcat(str,command);
                  }else if(i == 2){
                    strcat(str,list[1]);
                    i--;
                  }
                   send(sock, str, strlen(str), 0);
                   read(sock, buffer, 40);
                   if (strcmp(buffer, "Ok!") == 0) printf("\n%s\n", buffer);
                  }        
                
                else if(strcmp(list[0], "put") == 0){
                  int sent = 1;
                  do{
                     memset(buffer, 0, sizeof(buffer));
                     char str[] = "PUTMG!";
                     if (i == 1){
                        printf("put:>\n");
                        scanf("%s", command);
                        strcat(str, command);
                    }else if (i == 2){
                        strcat(str, list[1]);
                        strcat(str, "!");
                        i--;
                        printf("put:>\n");
                        scanf("%s", command);
                        strcat(str, command);
                    }else if(i == 3){
                       strcat(str, list[1]);
                       strcat(str, "!");
                       strcat(str, list[2]);
                       i = i-2;
                     }
                    send(sock, str, strlen(str), 0);
                    read(sock, buffer, 40);
                    if (strcmp(buffer, "Ok!") == 0){
                        printf("%s\n", buffer);
                        sent = 0;
                   }else{
                        printf("%s\n", buffer);
                   }         
                  }while(sent);
               
                }
              //Other commands
            }else{
                printf("A session with DUMB server was not intialized, please use the command HELLO to start.\n");
            }
        }
       
        //HELLO, help, and quit commands
        else{
            if(strcmp(command, "HELLO\n") == 0){
                printf("Here\n");
                send(sock, "HELLO", 5, 0);
                read(sock, buffer, 40);
                printf("Buffer is %s\n", buffer);
                if (strcmp(buffer, "HELLO DUMBv0 ready!") == 0){
                    if (instanceIntiaited){
                        printf("An instance is already initalized, if you would like to close this instance please use the command 'quit'.\n");
                    }else{
                        printf("%s \n", buffer);
                        instanceIntiaited = 1;
                    }
                }else{
                  printf("Something went wrong closing the connection\n");
                  serverIsLive = 0;
                }
            }else if (strcmp(command, "quit\n") == 0){
                send(sock, "GDBYE", 5, 0);
                if (read(sock, buffer, 40) < 1){
                    printf("socket can not be read from and was closed on server side\n");
                }else{
                    printf("Something went wrong when disconnecting from server, closing client.\n");
                }
                serverIsLive = 0;
            }else if (strcmp(command, "help") == 0){
                printf("Commands are: \n hello \n quit \n create \n delete \n open\n close\n next\n put\n");
            }else{
                printf("This is an invalid command, please type 'help' for a list of commands \n");
            }
        }
        //Clean out buffer
        memset(buffer, 0, sizeof(buffer));
    }while (serverIsLive);

   
    //Read from server
    // char buffer[40];
    //read(sock,buffer,sizeof(buffer)-1);

    //Close
    close(sock); 
  
    return 0;
}