#include "creature.h"
#include "board.h"
#include <stdlib.h>

#define GEN_FOOD_FOXES 10
#define GEN_PROC_FOXES 5
#define GEN_PROC_RABBITS 5
#define NOTHING ' '

extern bool verbose;

creature_t *initCreature(world_t *world, int row, int col, char type) {
  creature_t *creature;
  creature = (creature_t*)malloc(sizeof(creature_t));
  creature->id = -1;
  creature->row = row;
  creature->col = col;
  creature->age = 0;
  creature->food = 0;
  creature->alive = true;
  if (type == 'F') {
    world->board[row][col]->type = 'F';
    creature->species = "Fox";
    creature->food = GEN_FOOD_FOXES;
  } else if (type == 'R') {
    world->board[row][col]->type = 'R';
    creature->species = "Rabbit";
  }
  return creature;
};

creature_t *newCreature(world_t *world, int row, int col, char type) {
  world->creatures++;
  cell_t* cell = world->board[row][col];
  list_t* list = world->creaturesList;
  creature_t *creature;
  cell->creature = initCreature(world, row, col, type);
  creature = world->board[row][col]->creature;
  creature->genCreated = world->gen;
  if (list->size == 0)
    creature->id = 0;
  else
    creature->id = list->last->creature->id + 1;
  addtoList(world, creature);
  if (type == 'F') {
    world->foxes++;
    if (verbose)
      printf("New %s (id%d) in position (%d,%d)\n", creature->species, creature->id, row, col);
  } else if (type == 'R') {
    world->rabbits++;
    if (verbose)
      printf("New %s (id%d) in position (%d,%d)\n", creature->species, creature->id, row, col);
  }
  return creature;
}

void removeCreature(cell_t *cell) {
  cell->creature = NULL;
  cell->type = NOTHING;
}

void killCreature(world_t *world, cell_t *cell) {
  creature_t *creature = cell->creature;
  creature->alive = false;
  world->creatures--;
  if (cell->type == 'F') {
    world->foxes--;
    if (verbose)
      printf("Fox %d in position (%d,%d) died :(\n", creature->id,
             cell->creature->row, cell->creature->col);
  } else if (cell->type == 'R') {
    world->rabbits--;
    if (verbose)
      printf("Rabbit %d in position (%d,%d) died :(\n", creature->id,
             cell->creature->row, cell->creature->col);
  }
  removeCreature(cell);
}

void tryProcreation(world_t *world, cell_t *cell) {
  creature_t *creature = cell->creature;
  char type = cell->type;
  if (type == 'F' && (creature->age % GEN_PROC_FOXES) == 0) {
    if (verbose)
      printf("%s %d had a baby! ", creature->species, creature->id);
    newCreature(world, cell->row, cell->col, type);
  } else if (type == 'R' && (creature->age % GEN_PROC_RABBITS) == 0) {
    if (verbose)
      printf("%s %d had a baby! ", creature->species, creature->id);
    newCreature(world, cell->row, cell->col, type);
  } else {
    removeCreature(cell);
  }
}

void movement(world_t *world, creature_t *creature, char type) {
  cell_t *up = NULL;
  cell_t *down = NULL;
  cell_t *left = NULL;
  cell_t *right = NULL;
  int row = creature->row;
  int col = creature->col;
  cell_t *current = world->board[row][col];
  if (row - 1 >= 0)
    up = world->board[row - 1][col];
  if (row + 1 < world->rows)
    down = world->board[row + 1][col];
  if (col - 1 >= 0)
    left = world->board[row][col - 1];
  if (col + 1 < world->cols)
    right = world->board[row][col + 1];
  if (up != NULL && up->creature == NULL) {
    creature->row--;
    up->type = type;
    up->creature = creature;
    if (verbose)
      printf("%s %d moved from (%d,%d) to (%d,%d)\n", creature->species,
             creature->id, row, col, creature->row, creature->col);
    tryProcreation(world, current);
    ;
  } else if (down != NULL && down->type == NOTHING) {
    creature->row++;
    down->type = type;
    down->creature = creature;
    if (verbose)
      printf("%s %d moved from (%d,%d) to (%d,%d)\n", creature->species,
             creature->id, row, col, creature->row, creature->col);
    tryProcreation(world, current);
  } else if (left != NULL && left->type == NOTHING) {
    creature->col--;
    left->type = type;
    left->creature = creature;
    if (verbose)
      printf("%s %d moved from (%d,%d) to (%d,%d)\n", creature->species,
             creature->id, row, col, creature->row, creature->col);
    tryProcreation(world, current);
  } else if (right != NULL && right->type == NOTHING) {
    creature->col++;
    right->type = type;
    right->creature = creature;
    if (verbose)
      printf("%s %d moved from (%d,%d) to (%d,%d)\n", creature->species,
             creature->id, row, col, creature->row, creature->col);
    tryProcreation(world, current);
  } else {
    if (verbose)
      printf("%s %d stayed in (%d,%d)\n", creature->species, creature->id, row,
             col);
  }
}

void eatCreature(world_t *world, creature_t *predator, char type, char target) {
  cell_t *up = NULL;
  cell_t *down = NULL;
  cell_t *left = NULL;
  cell_t *right = NULL;
  int row = predator->row;
  int col = predator->col;
  cell_t *current = world->board[row][col];
  if (predator->alive == false)
    return;
  if (row - 1 >= 0)
    up = world->board[row - 1][col];
  if (row + 1 > world->rows)
    down = world->board[row + 1][col];
  if (col - 1 >= 0)
    left = world->board[row][col - 1];
  if (col + 1 > world->cols)
    right = world->board[row][col + 1];
  if (up != NULL && up->type == target) {
    if (verbose)
      printf("%s %d ate %s %d and moved from (%d,%d) to (%d,%d)\n",
             predator->species, predator->id, up->creature->species,
             up->creature->id, row, col, row-1, col);
    killCreature(world, up);
    predator->food = GEN_FOOD_FOXES;
    predator->row--;
    up->type = type;
    up->creature = predator;
    tryProcreation(world, current);
  } else if (down != NULL && down->type == target) {
    if (verbose)
      printf("%s %d ate %s %d and moved from (%d,%d) to (%d,%d)\n",
             predator->species, predator->id, down->creature->species,
             down->creature->id, row, col, row+1, col);
    killCreature(world, down);
    predator->food = GEN_FOOD_FOXES;
    predator->row++;
    down->type = type;
    down->creature = predator;
    tryProcreation(world, current);
  } else if (left != NULL && left->type == target) {
    if (verbose)
      printf("%s %d ate %s %d and moved from (%d,%d) to (%d,%d)\n",
             predator->species, predator->id, left->creature->species,
             left->creature->id, row, col, row, col-1);
    killCreature(world, left);
    predator->food = GEN_FOOD_FOXES;
    predator->col--;
    left->type = type;
    left->creature = predator;
    tryProcreation(world, current);
  } else if (right != NULL && right->type == target) {
    if (verbose)
      printf("%s %d ate %s %d and moved from (%d,%d) to (%d,%d)\n",
             predator->species, predator->id, right->creature->species,
             right->creature->id, row, col, row, col+1);
    killCreature(world, right);
    predator->food = GEN_FOOD_FOXES;
    predator->col++;
    right->type = type;
    right->creature = predator;
    tryProcreation(world, current);
  } else {
    if (predator->food == 0) {
      if (verbose)
        printf("Starvation! ");
      killCreature(world, world->board[predator->row][predator->col]);
    } else {
      movement(world, predator, type);
      predator->food--;
    }
  }
}

/*
 Rabbits can move horizontally or vertically (i.e., north, east, south and west
 directions), but not diagonally. In each generation, rabbits attempt to move
 themselves to an empty adjacent cell. If there are many empty adjacent cells,
 they choose one accordingly to a common criteria for selecting adjacent cells
 (see below). If there is no empty adjacent cell, they stay in the same place.
*/
void rabbitMovement(world_t *world, creature_t *rabbit) {
  movement(world, rabbit, 'R');
}

// Foxes can move horizontally or vertically, but not diagonally.
void foxMovement(world_t *world, creature_t *fox) {
  eatCreature(world, fox, 'F', 'R');
}

/*
  Rabbits can procreate whenever GEN_PROC_RABBITS generations have passed since
  they were born or since they last procreated. Whenever a rabbit reaches such
  age (to procreate) and makes a move, it leaves in its last position a new
  rabbit and both rabbits' procreation age is set to zero.
*/

/*
  Foxes can procreate whenever GEN_PROC_FOXES generations have passed since
  they were born or since they last procreated. Whenever a fox reaches such
  age (to procreate) and makes a move, it leaves in its last position a new
  fox and both foxes' procreation age is set to zero.
*/

/*
  Foxes starve and die whenever GEN_FOOD_FOXES generations have passed since
  they were born or since they last ate a rabbit. Foxes die after not finding
  a rabbit to eat and before attempting to move themselves to an empty adjacent
  cell.

  In each generation, foxes try to eat a rabbit by moving to an adjacent cell
  that is occupied by a rabbit. If there are many adjacent cells occupied with
  rabbits, they choose one accordingly to the common criteria for selecting
  adjacent cells (see below). If there is no adjacent cell occupied by a rabbit,
  foxes attempt to move themselves to an empty adjacent cell by following the
  same criteria. If there is no adjacent cell occupied with a rabbit or empty,
  they stay in the same place.
*/
