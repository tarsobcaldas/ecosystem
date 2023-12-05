#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdio.h>
#include "creature.h"
#include "list.h"

#define NOTHING ' '

struct creature_t;
struct list_t;

extern bool verbose, silent;
extern int foxHunger, foxRepr, rabbitRepr, maxGen, nObjects;
extern char* inputFile, *outputFile;

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
  struct cell_t*** nextGenBoard;
  struct list_t* rabbitsList;
  struct list_t* foxesList;
} world_t;

cell_t *initCell(int row, int col, char type);
cell_t ***initBoard(int rows, int cols);

world_t* initWorld(world_t* world, int rows, int cols);
world_t* populateFromInput(world_t* world);

void readBoard(world_t* world, FILE* input);
void populateBoard(world_t* world, int foxes, int rabbits, int rocks);
void move(world_t* world, cell_t* cell);
void newGeneration(world_t* world);
void printBoard(world_t* world);
void printStatus(world_t* world, FILE* output);
void destroyBoard(world_t* world);

#endif
