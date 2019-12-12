#ifndef DUMB_H
#define DUMB_H

typedef struct msg message; 
struct msg{ 
    char * message; 
    message * next; 
}; 
  
typedef struct box Box;
struct box { 
    message *front, *rear; 
}; 
  

typedef struct node Node;
  struct node{
	char * name;
  Box * msg;
  Node * next;
  int open; //Open is 1 Closed is 0
};
    


#endif