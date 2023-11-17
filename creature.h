#ifndef CREATURE_H
#define CREATURE_H

#include <stdbool.h>

struct world_t;
struct cell_t;

typedef struct creature_t {
  int  id;
  int row;
  int col;
  int age;
  int food;
  bool alive;
  char* species;
  int genCreated;
} creature_t;

creature_t* initCreature(struct world_t* world, int row, int col, char type);
creature_t* newCreature(struct world_t* world, int row, int col, char type);

void removeCreature(struct cell_t* cell);
void killCreature(struct world_t* world, struct cell_t* cell);
void movement(struct world_t* world, creature_t* creature, char type);
void eatCreature(struct world_t* world, creature_t* predator, char type, char target);
void rabbitMovement(struct world_t* world, creature_t* rabbit);
void foxMovement(struct world_t* world, creature_t* fox);

#endif
