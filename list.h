#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <omp.h>

#define LIST_PARALLEL_THRESHOLD 50 

struct creature_t;
struct world_t;

extern bool verbose;
extern char* outputFile;
extern FILE* output;

typedef struct node_t {
  struct creature_t* creature;
  struct node_t* next;
} node_t;

typedef struct list_t {
  int size;
  char type;
  omp_lock_t lock;
  struct node_t* first;
  struct node_t* last;
} list_t;

list_t* initList(char type);
node_t* getFromList(struct list_t* list, int id);
void addtoList(struct list_t* list, struct creature_t* creature);
void printList(struct world_t* list);
void removefromList(struct list_t* list, struct node_t* node);
void cleanList(struct list_t* list);
void destroyList(struct list_t* list);

#endif
