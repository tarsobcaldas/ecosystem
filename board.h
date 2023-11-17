#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdio.h>
#include "creature.h"
#include "list.h"

struct creature_t;
struct list_t;

typedef struct cell_t {
  int row;
  int col;
  char type;
  struct creature_t* creature;
} cell_t;

typedef struct world_t {
  long gen;
  int rows;
  int cols;
  int foxes;
  int rabbits;
  int rocks;
  int creatures;
  struct cell_t*** board;
  struct list_t* creaturesList;
} world_t;

world_t* initBoard(world_t* world, int rows, int cols);
world_t* readInputFile(world_t* world, char* filename);

void readBoard(world_t* world, FILE* input);
void populateBoard(world_t* world, int foxes, int rabbits, int rocks);
void move(world_t* world, cell_t* cell);
void newGeneration(world_t* world);
void printBoard(world_t* world);
void printStatus(world_t* world);
void destroyBoard(world_t* world);

#endif
