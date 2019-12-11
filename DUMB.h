#ifndef DUMB_H
#define DUMB_H

typedef struct message{
  char * message;
  struct message * next;
}MSG;


typedef struct node Node;
    struct node{
    char * name;
    MSG * msg;
    Node * next;
    int n; // Counts 
    int open; //Open is 1 Closed is 0
};


typedef struct connector{
  Node * head;
  int sock;
}con;

#endif