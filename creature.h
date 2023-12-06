#ifndef CREATURE_H
#define CREATURE_H

#include <stdbool.h>

#define NOTHING ' '

extern bool verbose;
extern int foxHunger, foxRepr, rabbitRepr;

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
  int reprAge;
  struct cell_t* previousPosition;
} creature_t;

creature_t* initCreature(struct cell_t*** board, int row, int col, char type);
creature_t* newCreature(struct world_t* world, struct cell_t*** board, int row, int col, char type);

void removeCreature(struct creature_t* cell);
void killCreature(struct world_t* world, struct cell_t* cell);
void movement(struct world_t* world, creature_t* creature, char type);
void eatCreature(struct world_t* world, creature_t* predator, char type, char target);
void rabbitMovement(struct world_t* world, creature_t* rabbit);
void foxMovement(struct world_t* world, creature_t* fox);

#endif
