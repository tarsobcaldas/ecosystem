#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

struct creature_t;
struct world_t;


typedef struct node_t {
  struct creature_t* creature;
  struct node_t* next;
} node_t;

typedef struct list_t {
  struct node_t* first;
  struct node_t* last;
  int size;
} list_t;

list_t* initList(struct world_t* world);
void addtoList(struct world_t* world, struct creature_t* creature);
void printList(struct world_t* world);
void removefromList(struct world_t* world, struct node_t* node);
void cleanList(struct world_t* world);
void destroyList(struct world_t* world);

#endif
