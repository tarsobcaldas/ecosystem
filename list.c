#include "list.h"
#include "board.h"
#include "creature.h"
#include <stdio.h>
#include <stdlib.h>

list_t *initList(char type) {
  list_t *list = (list_t *)malloc(sizeof(list_t));
  list->first = NULL;
  list->last = NULL;
  list->size = 0;
  list->type = type;
  return list;
}

void addtoList(list_t *list, creature_t *creature) {
  omp_set_lock(&list->lock);
  node_t *node = (node_t *)malloc(sizeof(node_t));
  node->creature = creature;
  node->next = NULL;
  if (list->size == 0) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    list->last = node;
  }
  list->size++;
  omp_unset_lock(&list->lock);
}

void removefromList(list_t *list, node_t *node) {
  omp_set_lock(&list->lock);
  node_t *prev = NULL;
  node_t *next = NULL;
  if (node == list->first) {
    list->first = node->next;
    if (node == list->last)
      list->last = NULL;
  } else if (node == list->last) {
    prev = list->first;
    while (prev->next != list->last)
      prev = prev->next;
    list->last = prev;
    prev->next = NULL;
  } else {
    prev = list->first;
    while (prev->next != node)
      prev = prev->next;
    next = node->next;
    prev->next = next;
  }
  omp_unset_lock(&list->lock);
#pragma omp critical
  list->size--;
  free(node);
}

void cleanList(list_t *list) {
  if (verbose)
    printf("Cleaning list...\n");
  node_t *node = list->first;
  node_t *next = NULL;
  while (node != NULL) {
#pragma omp parallel if (list->size > LIST_PARALLEL_THRESHOLD)
    {
#pragma omp single
      {
        next = node->next;
#pragma omp task
        {
          if (node->creature->alive == false) {
            if (verbose)
              printf("%s %d is dead, removing from list.\n",
                     node->creature->species, node->creature->id);
            removefromList(list, node);
          }
        }
        node = next;
      }
    }
  }
#pragma omp taskwait
  if (verbose)
    printf("List cleaned!\n\n");
}

void destroyList(list_t *list) {
  node_t *node = list->first;
  node_t *next = NULL;
  while (node != NULL) {
    next = node->next;
    free(node);
    node = next;
  }
  omp_destroy_lock(&list->lock);
  free(list);
}

void printList(world_t *world) {
  FILE *output = stdout;
  if (outputFile != NULL)
    output = fopen(outputFile, "a");
  int i = 1;
  list_t *list = world->rabbitsList;
  node_t *node = list->first;
  fprintf(output, "List of rabbits:\n");
  fprintf(output, "\tRow\tCol\tAge\tFood\tAlive\t id\n");
  while (node != NULL) {
    fprintf(output, "%d\t%d\t%d\t%d\t %d\t  %d\t%d\n", i, node->creature->row,
            node->creature->col, node->creature->age, node->creature->food,
            node->creature->alive, node->creature->id);
    node = node->next;
    i++;
  }
  printf("\n");
  list = world->foxesList;
  node = list->first;
  fprintf(output, "List of foxes:\n");
  fprintf(output, "\tRow\tCol\tAge\tFood\tAlive\tid\n");
  while (node != NULL) {
    fprintf(output, "%d\t%d\t%d\t%d\t %d\t  %d\t%d\n", i, node->creature->row,
            node->creature->col, node->creature->age, node->creature->food,
            node->creature->alive, node->creature->id);
    node = node->next;
    i++;
  }
  printf("\n");
}
