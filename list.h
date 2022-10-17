#include "libs.h"

#ifndef LIST_H
#define LIST_H

typedef void* Lista;

// Creates a list and returns it
void* createList();

// Free all the received list
void freeList(void* listtofree);

// Create a node and associate it with the received data, returning the node
void* createNode(void* data);

// Insert the given node in the tail of the given list
void insertAtTail(void* listtoadd, void* nodetoadd);

// Remove the given node from the given list
void removeNode(void* listtoadd, void* nodetoremove);

/*>>>>>>>>>>All sets<<<<<<<<<<*/

// Set the received node as the head of the given list
void setHead(void* listtoset, void* nodetoset);

/*>>>>>>>>>>All gets<<<<<<<<<<*/
/*Returns the specified data from the given node*/

void* getData(void* nodetoget);

void* getNext(void* nodetoget);

void* getHead(void* listtoget);

int getSize(void* listtoget);

#endif