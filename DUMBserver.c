#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "DUMB.h"

int serverRunning = 1;
pthread_mutex_t lock;
//Node * dummyHead;

void printLL(Node * dummyHead){
   Node * head = dummyHead;
    Node * temp = dummyHead;
    while(temp != NULL){
      printf("Printing: %s\n",temp->name);
      temp = temp->next;
    }
  return;
}

int counter(Node * dummyHead){
  int i = 0;
  Node * temp = dummyHead;
  while(dummyHead->next != NULL){
    i++;
    dummyHead = dummyHead->next;
  }
  return i;
}
//Return 1 found
//Return 0 NOT EMPTY can't delete
//Return -1 Still Open can't delete
//Return -2 Name doesn't exist
int deleter(Node * dummyHead,char * name){
  Node * temp = dummyHead;
  if(temp->next == NULL){//It's an empty list
    return -2;
  }
  Node * prev = temp;
  temp = temp->next;
  while(temp != NULL){
    if (strcmp(temp->name, name) == 0){//Same name
        if(temp->msg == NULL){ //No messages
          if(temp->open == 0){ //Closed
            prev->next = temp->next;
            return 1;
          }else{
            return -1;
          }
        }else{
          return 0;
        }
     }
    prev = temp;
    temp = temp->next;
  }
  return -2; //Was not found
  
  
}

/*//return 1 if worked return 0 if name already exists
int add(Node * dummyHead, char *name){
    printf("There are %d message boxes\n",counter(dummyHead));
    printf("In Add\n");
    Node *temp = dummyHead;
    Node * result = (Node *) malloc(sizeof(Node));
    result->name = name;
    result->next = NULL;
    result->open = 0;
    if(temp->next == NULL){
      printf("Here\n");
      temp->next = result;
     // printf("Temp->next = %s\n",temp->next->name);
       printf("Result2->name = %s\n", temp->next->name);
      return 1;
    }
    temp = temp->next;
    printf("temp->next = %s\n", temp->name);
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
     printf("Result1->name = %s\n", temp->next->name);
    return 1;
}*/

int findName(Node * dummyHead, char * name){
  Node * temp = dummyHead;
  while(temp != NULL){
    if(strcmp(temp->name,name) == 0){
      return 1;
    }
    temp = temp->next;
  }
  return 0;
}


//return 1 if worked return 0 if name already exists
int add(Node * dummyHead, char *name){
  printf("There are %d message boxes\n",counter(dummyHead));
  printf("In Add\n");
  char * ptr = malloc(sizeof(char) * strlen(name)+1);
  strcpy(ptr,name);
  if(findName(dummyHead,name)){
    printf("Found a name\n");
    return 0;
  }
  Node * result = (Node *) malloc(sizeof(Node));
  result ->next = NULL;
  result-> name = ptr;
  result->open = 0;
  Node * temp = dummyHead;
  while(temp->next != NULL){
    temp = temp->next;
  }
  temp->next = result; 
  return 1;
}


//return 1 if worked, return 0 if name already opened, return -1 if name doesn't exist
int opener(Node * dummyHead,char * name){
  Node * temp = dummyHead;
  if(temp->next == NULL){//It's an empty list
    return -1;
  }
  temp = temp->next;
  while(temp != NULL){
    if (strcmp(temp->name, name) == 0){
      printf("temp is %s\n", temp->name);
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
int closer(Node * dummyHead, char * name){
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
        int client_sock = (uintptr_t)arg;
        char buffer[40];        
        Node *dummyHead = (Node *) malloc(sizeof(Node));
        dummyHead->next = NULL;
        dummyHead->name = "head"; //This is fine as it's not >5 chars
        do{                 
        printLL(dummyHead);
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
        //pthread_mutex_lock(&lock);

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
                //pthread_mutex_lock(&lock);
                 
                  if (add(dummyHead,list[1]) == 1){
                  //  pthread_mutex_unlock(&lock);
                      write(client_sock, "Ok!", 3);
                      printf("Creates a message box\n");
                 }else{
                  write(client_sock, "ER:EXIST", 8);
               }
              }

            //OPNBX
            else if(strcmp(list[0], "OPNBX") == 0){ 
                    printf("In OPNBX\n");
                    int opened = opener(dummyHead,list[1]);
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
              if(closer(dummyHead,list[1]) == 1){
                write(client_sock, "Ok!", 3);
              }else{
                 write(client_sock, "ER:NOOPN", 8);
              }              
            }
                
            //DELBX
            //Return 1 found
            //Return 0 NOT EMPTY can't delete :NOTMT
            //Return -1 Still Open can't delete: OPEND
            //Return -2 Name doesn't exist: NEXST
            else if(strcmp(list[0], "DELBX") == 0){
              printf("In DELBX\n");
              int deleted = deleter(dummyHead,list[1]);
              if(deleted == 1){
                write(client_sock, "Ok!", 3);
              }else if(deleted == 0){
                write(client_sock, "ER:NOTMT", 8);
              }else if(deleted == -1){
                write(client_sock, "ER:OPEND", 8);
              }else{
                write(client_sock, "ER:NEXST", 8);
              }
              
            }
                
            //Other commands     
                
                
          }
        }else if (i == 3){
        }else{
            write(client_sock, "ER:WHAT?", 8);
        }

        memset(buffer, 0, sizeof(buffer));
        }while(serverRunning);
        close(client_sock);
       // pthread_mutex_unlock(&lock);
        printf("Exiting a conneciton...");
      free(dummyHead);

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
     
    pthread_t tid;
    pthread_mutex_init(&lock, NULL);
    while(client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size)){
      printf("Conencting to client...\n");
      pthread_create(&tid, NULL,run,(void*) (uintptr_t) client_sock);
      pthread_detach(tid);
    }
  
    close(server_sock);
  // pthread_mutex_destroy(&lock);
    return 0;
}