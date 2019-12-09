#ifndef DUMB_H
#define DUMB_H

typedef struct node Node;
struct node{
	char* name;
  MSG * next;
  Node * next;
};

typedef struct message MSG;
struct message{
  char * message;
  MSG * next;
};

#endif