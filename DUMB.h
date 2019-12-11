#ifndef DUMB_H
#define DUMB_H

typedef struct msg message{
  struct msg{
  char * text;
  message * next;
};


typedef struct node Node;
  struct node{
	char * name;
  message * msg;
  Node * next;
  int open; //Open is 1 Closed is 0
};

#endif