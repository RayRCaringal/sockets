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

void enqueue(Box * b, char * msg){
  message * new = (message *)malloc(sizeof(message)); 
    new->message = msg; 
    new->next = NULL;
    if (b->rear == NULL) { 
        b->front = new;
        b->rear = new; 
        return; 
    } 
    b->rear->next = new; 
    b->rear = new; 
} 

message * dequeue(Box * b) { 
    if (b->front == NULL)return NULL;   
    message * temp = b->front; 
    free(temp); 
    b->front = b->front->next; 
    if (b->front == NULL)b->rear = NULL; 
    return temp; 
} 


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
  while(dummyHead != NULL){
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

Node * findName(Node * dummyHead, char * name){
  Node * temp = (Node *)malloc(sizeof(Node));
    temp = dummyHead;
  while(temp != NULL){
    if(strcmp(temp->name,name) == 0){
      return temp;
    }
    temp = temp->next;
  }
  return NULL;
}


//return 1 if worked return 0 if name already exists
int add(Node * dummyHead, char *name){
  printf("There are %d message boxes\n",counter(dummyHead));
  printf("In Add\n");
  char * ptr = malloc(sizeof(char) * strlen(name)+1);
  strcpy(ptr,name);
  if(findName(dummyHead,name) != NULL){
    printf("Found a name\n");
    return 0;
  }
  Node * result = (Node *) malloc(sizeof(Node));
  result ->next = NULL;
  result-> name = ptr;
  result->open = 0;
  result->msg = NULL;
  Node * temp = dummyHead;
  while(temp->next != NULL){
    temp = temp->next;
  }
  temp->next = result; 
  return 1;
}


/*//return 1 if worked, return NULL if name doesn't exist
Node * opener(Node * dummyHead,char * name){
  if(dummyHead->next == NULL){//It's an empty list
    return NULL;
  }
  Node * temp = (Node *) malloc(sizeof(Node));
  temp = dummyHead;
  temp = temp->next;
  while(temp != NULL){
    if (strcmp(temp->name, name) == 0){
      if(temp->open == 0){
        temp->open = 1;
        return temp;
      }
     }
    temp = temp->next;
  }
  return NULL;
}*/

//return 1 on success, return 0 otherwise
void putMessage(Node * openedHead, char * message){
     printf("In Put\n");
    Node * temp = openedHead;
    if(temp->msg == NULL){ //No message yet
        temp->msg = (Box *) malloc(sizeof(Box));
        temp->msg->rear = NULL;
        temp->msg->front = NULL;
     }
    enqueue(temp->msg,message);
    return;
  }


void * run(void * arg){
        int client_sock = (uintptr_t)arg;
        char buffer[40];        
        Node *dummyHead = (Node *) malloc(sizeof(Node));
        Node *openedHead; 
        dummyHead->next = NULL;
        dummyHead->name = "head"; //This is fine as it's not >5 chars
        do{                 
        printLL(dummyHead);
        memset(buffer, 0, sizeof(buffer));
        recv(client_sock, buffer, sizeof(buffer), 0);
        char *list[4];
        int i = 0;
        printf("Buffer is %s\n", buffer);
        if(strchr(buffer, '!') >0){//Is put 
          list[i] =strtok(buffer,"!");
          while (list[i] != NULL){
            i++;
            list[i] = strtok(NULL, "!");
        }
        }else{
        list[i] = strtok(buffer, " ");
        while (list[i] != NULL){
            i++;
            list[i] = strtok(NULL, " ");
        }
  
        }
        
        printf("%s\n", buffer);
        printf("I is %d\n", i);
        //pthread_mutex_lock(&lock);

        if (i == 1){
            if (strcmp(list[0], "HELLO") == 0){
                write(client_sock, "HELLO DUMBv0 ready!", 19);
            }else if (strcmp(list[0], "GDBYE") == 0){
                serverRunning = 0;
            }else if(strcmp(list[0], "NEXTMG") == 0){
                printf("In Next\n"); 
            }else if(strcmp(list[0], "CLSBX") == 0){
              printf("In CLSBX\n");
             if(openedHead != NULL){
               openedHead->open = 0;
               openedHead = NULL;
               printf("Closed\n");
               write(client_sock, "Ok!", 3);
             }else{
                 printf("NOOPN\n");
                 write(client_sock, "ER:NOOPN", 8);
              }              
            }else{
                write(client_sock, "ER:EMPTY", 19);
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
                   printf("EXIST\n");  
                  write(client_sock, "ER:EXIST", 8);
               }
              }

            //OPNBX
            else if(strcmp(list[0], "OPNBX") == 0){ 
                    printf("In OPNBX\n");
                    openedHead = findName(dummyHead,list[1]);
                    if(openedHead != NULL){
                       if(openedHead->open == 0){
                         openedHead->open=1;
                         write(client_sock, "Ok!", 3);
                         printf("Opened %s\n", list[1]);
                       }else{
                        printf("Already opened\n");
                        write(client_sock, "ER:OPEND", 8);   
                       }
                    }else{
                       printf("Does not exist\n");
                       write(client_sock, "ER:NEXIST", 8);
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
                printf("Deleted\n");
                write(client_sock, "Ok!", 3);
              }else if(deleted == 0){
                printf("NOTMT\n");
                write(client_sock, "ER:NOTMT", 8);
              }else if(deleted == -1){
                printf("OPEND\n");
                write(client_sock, "ER:OPEND", 8);
              }else{
                printf("NEXST\n");
                write(client_sock, "ER:NEXST", 8);
              }              
            }
                
            //Other commands     
                
                
          }
        }else if (i == 3){
          //It has to be put 
          if(strcmp(list[0],"PUTMG")==0){
            printf("In PUT\n"); 
            if(openedHead != NULL){
              putMessage(openedHead,list[2]);
              printf("Put successfuly\n"); 
              write(client_sock, "Ok!", 3);                
              
            }else{
               printf("ER:NOOPN\n");
               write(client_sock, "ER:NOOPN!", 3);
           }
          }else{
            write(client_sock, "ER:WHAT?", 8);
          }
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