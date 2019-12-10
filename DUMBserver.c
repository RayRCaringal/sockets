#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "DUMB.h"

Node *dummyHead;
//return 1 if worked return 0 if name already exists

int counter(){
  int i = 0;
  Node * temp = dummyHead;
  while(dummyHead->next != NULL){
    i++;
    dummyHead = dummyHead->next;
  }
  return i;
}

int add(char *name){
    printf("There are %d message boxes\n",counter());
    printf("In Add\n");
    Node *temp = dummyHead;
    Node *result = NULL;
    result = (Node *) malloc(sizeof(Node));
    result->name = name;
    result->next = NULL;
    result->open = 0;
    if(temp->next == NULL){
      temp->next = result;
       printf("Result->name = %s\n", temp->next->name);
      return 1;
    }
    temp = temp->next;
    while (temp->next != NULL){
        if (strcmp(temp->name, name) == 0){
          printf("Temp->name = %s\n", temp->name);
          printf("First\n");
          return 0;
        }
          temp = temp->next;
    }
    if (strcmp(temp->name, name) == 0){
       printf("Temp->name = %s\n", temp->name);  
      printf("Second\n");
        return 0;
    }
     temp->next = result;
     printf("Result->name = %s\n", temp->next->name);
    return 1;
}

//return 1 if worked, return 0 if name already opened, return -1 if name doesn't exist
int opener(char * name){
  Node * temp = dummyHead;
  if(temp->next == NULL){//It's an empty list
    return -1;
  }
  temp = temp->next;
  while(temp != NULL){
    if (strcmp(temp->name, name) == 0){
      if(temp->open == 0){
        temp->open = 1;
        return 1;
      }else{
          return 0;
        }
     }
    temp = temp->next;
  }
  return -1;
}

//return 1 if closed successfuly, return 0 otherwise
int closer(char * name){
   Node * temp = dummyHead;
  if(temp->next == NULL){//It's an empty list
    return 0;
  }
  temp = temp->next;
  while(temp != NULL){
    if (strcmp(temp->name, name) == 0){
      if(temp->open == 1){
        temp->open = 0;
        return 1;
      }else{
          return 0; //Already closed 
        }
     }
    temp = temp->next;
  }
  return 0; //Was not found
}


void * run(void * arg){
        int client_sock = (intptr_t )arg;
        int serverRunning = 1;
        char buffer[40];        
        do{
        memset(buffer, 0, sizeof(buffer));
        recv(client_sock, buffer, sizeof(buffer), 0);
        char *list[4];
        int i = 0;
        printf("Buffer is %s\n", buffer);
        list[i] = strtok(buffer, " ");
        while (list[i] != NULL){
            i++;
            list[i] = strtok(NULL, " ");
        }

        printf("%s\n", buffer);
        printf("I is %d\n", i);

        if (i == 1){
            if (strcmp(buffer, "HELLO") == 0){
                write(client_sock, "HELLO DUMBv0 ready!", 19);
            }else if (strcmp(buffer, "GDBYE") == 0){
                serverRunning = 0;
            }else{
                //write(client_sock, "ER:EMPTY", 19);
            }
        }else if (i == 2){
              if (strlen(list[1]) < 5 || strlen(list[1]) > 25 || !(isalpha(list[1][0]))){
                      write(client_sock, "ER:WHAT?", 8);
                  }else{
                  //CREATE
              if (strcmp(list[0], "CREAT") == 0){
                  printf("In CREAT\n");
                  if (add(list[1]) == 1){
                      write(client_sock, "Ok!", 3);
                      printf("Creates a message box\n");
                      }else{
                          write(client_sock, "ER:EXIST", 8);
                      }               
              }

            //OPNBX
            else if(strcmp(list[0], "OPNBX") == 0){ 
                    printf("In OPNBX\n");
                    int opened = opener(list[1]);
                    if(opened == 1){
                       write(client_sock, "Ok!", 3);
                       printf("Opened %s\n", list[1]);
                    }else if(opened == 0){
                      printf("Was already opened\n");
                      write(client_sock, "ER:OPEND", 8);
                    }else{
                       printf("Does not exist\n");
                       write(client_sock, "ER:NEXIST", 8);
                    }

            }
            //CSLBX
            else if(strcmp(list[0], "CLSBX") == 0){
              printf("In CLSBX\n");
              if(closer(list[1]) == 1){
                write(client_sock, "Ok!", 3);
              }else{
                 write(client_sock, "ER:NOOPN", 8);
              }              
            }
            //Other commands     
                
                
          }
        }else if (i == 3){
        }
        else{
            write(client_sock, "ER:WHAT?", 8);
        }

        memset(buffer, 0, sizeof(buffer));
        }while(serverRunning);
        close(client_sock);
        return;
}



int main(int argc, char *argv[]){
    /*
 if(argc < 2){
    printf("There are not enough arguments\n");
  }
  char * ip = argv[1];
  int port = atoi(argv[2]);
  */

    //Create Socket
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    int z = 1;
    char *str;
    setsockopt(server_sock, SOL_SOCKET, SO_LINGER, &z, sizeof(z)); // actually kills the socket when it is cloed.
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &z, sizeof(z));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;                                        // IPv4
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");                    // Set IP address
    server_addr.sin_port = htons(1234);                                      // set port
    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)); // Bind

    listen(server_sock, 20);

    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_sock; 
    // recv(client_sock,buffer,sizeof(buffer),0);

    dummyHead = (Node *)malloc(sizeof(Node));
    dummyHead->next = NULL;
    dummyHead->msg = NULL;
    dummyHead->name = NULL;
    dummyHead->n = 0;
  
  
    pthread_t tid;
    while(client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size)){
      printf("Conencting to client...\n");
      pthread_create(&tid, NULL,run,(void*) (intptr_t)client_sock);
      
      pthread_detach(tid);
    }
  

    close(server_sock);
    free(dummyHead);
    return 0;
}