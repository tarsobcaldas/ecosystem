#include "list.h"
#include "board.h"
#include "creature.h"
#include <stdio.h>
#include <stdlib.h>

extern bool verbose;

// Init linked list
list_t* initList(world_t *world) {
  world->creaturesList = (list_t*)malloc(sizeof(list_t));
  list_t* list = world->creaturesList;
  list->first = NULL;
  list->last = NULL;
  list->size = 0;
  return list;
}

void addtoList(world_t *world, creature_t *creature) {
  list_t* list = world->creaturesList;
  node_t* node = (node_t*)malloc(sizeof(node_t));
  node->creature = creature;
  node->next = NULL;
  if(list->size == 0) {
    list->first = node;
    list->last = node;
  }
  else {
    list->last->next = node;
    list->last = node;
  }
  list->size++;
}

// Remove creature from the linked list, free memory, update size
// and update list order. give the id always incrementally from the 
// biggest member
void removefromList(world_t* world, node_t* node) {
  list_t* list = world->creaturesList;
  node_t* prev = NULL;
  node_t* next = NULL;
  if (node == list->first) {
    list->first = node->next;
    if (node == list->last)
      list->last = NULL;
  }
  else if (node == list->last) {
    prev = list->first;
    while (prev->next != list->last)
      prev = prev->next;
    list->last = prev;
    prev->next = NULL;
  }
  else {
    prev = list->first;
    while (prev->next != node)
      prev = prev->next;
    next = node->next;
    prev->next = next;
  }
  list->size--;
  free(node);
}

// Clean the list from all the dead creatures
void cleanList(world_t* world) {
  if (verbose)
    printf("Cleaning list...\n");
  list_t* list = world->creaturesList;
  node_t* node = list->first;
  node_t* next = NULL;
  while(node != NULL) {
    next = node->next;
    if (node->creature->alive == false) {
      if (verbose)
        printf("%s %d is dead, removing from list.\n", node->creature->species, node->creature->id);
      removefromList(world, node);
    }
    node = next;
  }
  if (verbose)
    printf("List cleaned!\n\n");
}


void destroyList(world_t* world) {
  list_t* list = world->creaturesList;
  node_t* node = list->first;
  node_t* next = NULL;
  while(node != NULL) {
    next = node->next;
    free(node);
    node = next;
  }
  free(list);
}

void printList(world_t *world) {
  int i = 1;
  list_t* list = world->creaturesList;
  node_t* node = list->first;
  printf("List of creatures:\n");
  printf("\tRow\tCol\tAge\tFood\tAlive\tType\t id\n");
  while (node != NULL) {
      printf("%d\t%d\t%d\t%d\t %d\t  %d\t%s\t %d\n", i,
             node->creature->row, node->creature->col,
             node->creature->age, node->creature->food,
             node->creature->alive, node->creature->species,
             node->creature->id);
    
    node = node->next;
    i++;
  }
  printf("\n");
}
