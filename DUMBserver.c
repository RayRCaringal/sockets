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
void * run(void * arg){
        int client_sock = (intptr_t )arg;
        int serverRunning = 1;
        char buffer[40];        
        do{
        memset(buffer, 0, sizeof(buffer));
        recv(client_sock, buffer, sizeof(buffer), 0);
        char *token;
        char *list[4];
        int i = 0;
        printf("Buffer is %s\n", buffer);
        list[i] = strtok(buffer, " ");
        while (list[i] != NULL){
            i++;
            list[i] = strtok(NULL, " ");
        }

        printf("%s\n", buffer);
        printf("Buffer length = %d\n", strlen(buffer));
        int test = strcmp(buffer, "HELLO");
        printf("It returns %d\n", test);
        printf("I is %d\n", i);

        if (i == 1){
            if (strcmp(buffer, "HELLO") == 0){
                printf("Here\n");
                write(client_sock, "HELLO DUMBv0 ready!", 19);
            }else if (strcmp(buffer, "GDBYE") == 0){
                serverRunning = 0;
            }else{
                write(client_sock, "ER:EMPTY", 19);
            }
        }else if (i == 2){
            printf("In here\n");
            //CREATE
            if (strcmp(list[0], "CREAT") == 0){
                printf("In CREAT\n");
                if (strlen(list[1]) < 5 || strlen(list[1]) > 25 || !(isalpha(list[1][0]))){
                    write(client_sock, "ER:WHAT?", 8);
                }else{
                    //ADD IMPLEMENTATION FOR MESSAGE BOXES
                    if (add(dummyHead, list[1]) == 1){
                        printf("Creates a message box\n");
                    }else{
                        write(client_sock, "ER:EXIST", 8);
                    }
                }
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

//return 1 if worked return 0 if name already exists
int add(Node *head, char *name){
    printf("In Add\n");
    Node *temp = head;
    Node *result = NULL;
    result = malloc(sizeof(Node));
    result->name = name;
    result->next = NULL;
    int i = 1;
    while (temp->next != NULL){
        if (strcmp(temp->name, name) == 0){
            return 0;
        }
        i++;
        temp = temp->next;
    }
    result->n = i;
    temp->next = result;
    return 1;
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
      pthread_create(&tid, NULL,run,(void*) (intptr_t)client_sock);
      
      pthread_detach(tid);
    }
  

    close(server_sock);
    free(dummyHead);
    return 0;
}