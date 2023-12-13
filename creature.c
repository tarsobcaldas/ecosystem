#include "creature.h"
#include "board.h"
#include <stdlib.h>
#include <string.h>

creature_t *initCreature(cell_t ***board, int row, int col, char type) {
  creature_t *creature;
  creature = (creature_t *)malloc(sizeof(creature_t));
  creature->id = -1;
  creature->row = row;
  creature->col = col;
  creature->age = 0;
  creature->food = 0;
  creature->reprAge = 0;
  creature->alive = true;
  creature->previousPosition = NULL;
  if (type == 'F') {
    board[row][col]->type = 'F';
    creature->species = "Fox";
    creature->food = foxHunger;
  } else if (type == 'R') {
    board[row][col]->type = 'R';
    creature->species = "Rabbit";
  }
  return creature;
};

creature_t *newCreature(world_t *world, cell_t ***board, int row, int col,
                        char type) {
  world->creatures++;
  list_t *list;
  cell_t *cell = board[row][col];
  creature_t *creature;
  cell->creature = initCreature(board, row, col, type);
  creature = cell->creature;
  creature->genCreated = world->gen;
  if (type == 'R') {
    list = world->rabbitsList;
    world->rabbits++;
  } else if (type == 'F') {
    list = world->foxesList;
    world->foxes++;
  }
  if (list->size == 0)
    creature->id = 0;
  else
    creature->id = list->last->creature->id + 1;
  addtoList(list, creature);
  if (verbose)
    printf("New %s (id%d) in position (%d,%d)\n", creature->species,
           creature->id, row, col);
  return creature;
}

void removeCreature(creature_t* creature) {
  creature = NULL;
}

void killCreature(world_t *world, cell_t *cell) {
  creature_t *creature = cell->creature;
  creature->alive = false;
  char* fox = "Fox";
  char* rabbit = "Rabbit";
  char* species = creature->species;
  world->creatures--;
  if (strcmp(species, fox) == 0) {
    world->foxes--;
  } else if (strcmp(species, rabbit) == 0) {
    world->rabbits--;
  }
  if (verbose)
    printf("%s %d in position (%d,%d) died :(\n", creature->species,
           creature->id, cell->creature->row, cell->creature->col);
  removeCreature(creature);
}

void tryProcreation(world_t *world, cell_t *cell) {
  creature_t *creature = cell->creature;
  cell_t ***nextGenBoard = world->nextGenBoard;
  cell_t *nextGenCell = nextGenBoard[cell->row][cell->col];
  char type = cell->type;
  char* fox = "Fox";
  char* rabbit = "Rabbit"; 
  char* species = creature->species;
<<<<<<< HEAD
  if (strcmp(species, fox) == 0 && creature->reprAge > foxRepr) {
    if (verbose)
      printf("%s %d had a baby! ", creature->species, creature->id);
    newCreature(world, nextGenBoard, nextGenCell->row, nextGenCell->col,
                type);
    creature->reprAge = 0;
  } else if (strcmp(species, rabbit) == 0 && creature->reprAge > rabbitRepr) {
    if (verbose)
      printf("%s %d had a baby! ", creature->species, creature->id);
    newCreature(world, nextGenBoard, nextGenCell->row, nextGenCell->col,
                type);
    creature->reprAge = 0;
=======
  if (creature->age > 0) {
    if (strcmp(species, fox) == 0 && creature->age % (foxRepr + 1) == 0) {
      if (verbose)
        printf("%s %d had a baby! ", creature->species, creature->id);
      newCreature(world, nextGenBoard, nextGenCell->row, nextGenCell->col,
                  type);
    } else if (strcmp(species, rabbit) == 0 && creature->age % (rabbitRepr + 1) == 0) {
      if (verbose)
        printf("%s %d had a baby! ", creature->species, creature->id);
      newCreature(world, nextGenBoard, nextGenCell->row, nextGenCell->col,
                  type);
    }
>>>>>>> 9c6f503d29dc57e599c498932f272859c48817da
  }
}

cell_t *chooseNextCell(world_t *world, cell_t *cell, char type) {
  int nextCellID;
  int possibilities = 0;
  int row = cell->row;
  int col = cell->col;
  int gen = world->gen - 1;
  cell_t ***board = world->board;
  cell_t ***nextGenBoard = world->nextGenBoard;
  cell_t *up = NULL;
  cell_t *down = NULL;
  cell_t *left = NULL;
  cell_t *right = NULL;
  cell_t *nextGenUp = NULL;
  cell_t *nextGenDown = NULL;
  cell_t *nextGenLeft = NULL;
  cell_t *nextGenRight = NULL;
  cell_t *nextGenCurrent = nextGenBoard[row][col];
  creature_t *creature = cell->creature;

  creature->previousPosition = cell;

  if (row - 1 >= 0) {
    up = board[row - 1][col];
    nextGenUp = nextGenBoard[row - 1][col];
    if (up->type == type)
      possibilities++;
  }

  if (col + 1 < world->cols) {
    right = board[row][col + 1];
    nextGenRight = nextGenBoard[row][col + 1];
    if (right->type == type)
      possibilities++;
  }

  if (row + 1 < world->rows) {
    down = board[row + 1][col];
    nextGenDown = nextGenBoard[row + 1][col];
    if (down->type == type)
      possibilities++;
  }

  if (col - 1 >= 0) {
    left = board[row][col - 1];
    nextGenLeft = nextGenBoard[row][col - 1];
    if (left->type == type)
      possibilities++;
  }

  if (possibilities == 0)
    return nextGenCurrent;
  if (verbose)
    printf("\nPossibilities: %d\n", possibilities);
  nextCellID = (row + col + gen) % (possibilities);
  if (verbose)
    printf("nextCellID: (%d + %d + %d) mod %d = %d\n", row, col, gen,
           possibilities, nextCellID);

  switch (nextCellID) {
  case 0:
    if (up != NULL && up->type == type)
      return nextGenUp;
    else if (right != NULL && right->type == type)
      return nextGenRight;
    else if (down != NULL && down->type == type)
      return nextGenDown;
    else if (left != NULL && left->type == type)
      return nextGenLeft;
  case 1:
    if (up != NULL && up->type == type) {
      if (right != NULL && right->type == type)
        return nextGenRight;
      else if (down != NULL && down->type == type)
        return nextGenDown;
      else if (left != NULL && left->type == type)
        return nextGenLeft;
    } else {
      if (right != NULL && right->type == type) {
        if (down != NULL && down->type == type)
          return nextGenDown;
        else if (left != NULL && left->type == type)
          return nextGenLeft;
      } else if (left != NULL && left->type == type)
        return nextGenLeft;
    }
  case 2:
    if (up != NULL && up->type == type) {
      if (right != NULL && right->type == type) {
        if (down != NULL && down->type == type)
          return nextGenDown;
        else if (left != NULL && left->type == type)
          return nextGenLeft;
      } else if (left != NULL && left->type == type)
        return nextGenLeft;
    } else if (left != NULL && left->type == type)
      return nextGenLeft;
  case 3:
    if (left != NULL && left->type == type)
      return nextGenLeft;
  default:
    return nextGenCurrent;
  }
}

void movement(world_t *world, creature_t *creature, char type) {
  if (creature->alive == false || creature->genCreated == world->gen)
    return;
  int row = creature->row;
  int col = creature->col;
  cell_t *current = world->board[row][col];
  cell_t *nextCell = chooseNextCell(world, current, NOTHING);
  creature_t *nextCreature = nextCell->creature;


  if (nextCell->type == creature->species[0]) {
    int creatureReprAge, nextCreatureReprAge;
    if (creature->species[0] == 'F') {
      creatureReprAge = creature->age % (foxRepr + 1);
      nextCreatureReprAge = nextCreature->age % (foxRepr + 1);
    } else if (creature->species[0] == 'R') {
      creatureReprAge = creature->age % (rabbitRepr + 1);
      nextCreatureReprAge = nextCreature->age % (rabbitRepr + 1);
    }

    if (creatureReprAge < nextCreatureReprAge || (creatureReprAge == nextCreatureReprAge && creature->food < nextCreature->food)) {
      creature_t *nextCreature = nextCell->creature;
        if (verbose)
          printf("%s %d (%d) crashed into %s %d (%d) in position (%d,%d) and died\n",
                 creature->species, creature->id, creatureReprAge, nextCreature->species,
                 nextCreature->id, nextCreatureReprAge, nextCell->row, nextCell->col);
        tryProcreation(world,current);
        killCreature(world, current);
        return;
    } else {
      if (verbose)
        printf("%s %d (%d) killed %s %d (%d) and moved from (%d,%d) to (%d,%d)\n",
               creature->species, creature->id, creatureReprAge, nextCreature->species,
               nextCreature->id, nextCreatureReprAge, row, col, nextCell->row, nextCell->col);
      killCreature(world, nextCell);
      creature->row = nextCell->row;
      creature->col = nextCell->col;
      nextCell->type = type;
      nextCell->creature = creature;
      tryProcreation(world, current);
      return;
    }
  }

  if (nextCell->row == row && nextCell->col == col) {
    if (verbose)
      printf("\n%s %d stayed in (%d,%d)\n", creature->species, creature->id, row,
             col);
    nextCell->type = type;
    nextCell->creature = creature;
    return;
  } else {
    if (verbose)
      printf("%s %d moved from (%d,%d) to (%d,%d)\n", creature->species,
             creature->id, row, col, nextCell->row, nextCell->col);
    creature->row = nextCell->row;
    creature->col = nextCell->col;
    nextCell->type = type;
    nextCell->creature = creature;
    tryProcreation(world, current);
  }
}

bool preyOrCorpse(cell_t *cell, char type) {
  if (cell->type == type || cell->type == 'C')
    return true;
  return false;
}

cell_t *chooseNextPrey(world_t *world, cell_t *cell, char type) {
  int nextCellID;
  int possibilities = 0;
  int row = cell->row;
  int col = cell->col;
  int gen = world->gen - 1;
  cell_t ***nextGenBoard = world->nextGenBoard;
  cell_t *up = NULL;
  cell_t *down = NULL;
  cell_t *left = NULL;
  cell_t *right = NULL;
  creature_t *creature = cell->creature;

  creature->previousPosition = cell;

  if (row - 1 >= 0) {
    up = nextGenBoard[row - 1][col];
    if (preyOrCorpse(up, type))
      possibilities++;
  }

  if (col + 1 < world->cols) {
    right = nextGenBoard[row][col + 1];
    if (preyOrCorpse(right, type))
      possibilities++;
  }

  if (row + 1 < world->rows) {
    down = nextGenBoard[row + 1][col];
    if (preyOrCorpse(down, type))
      possibilities++;
  }

  if (col - 1 >= 0) {
    left = nextGenBoard[row][col - 1];
    if (preyOrCorpse(left, type))
      possibilities++;
  }

  if (possibilities == 0)
    return cell;
  if (verbose)
    printf("\nPossibilities: %d\n", possibilities);
  nextCellID = (row + col + gen) % (possibilities);
  if (verbose)
    printf("nextCellID: (%d + %d + %d) mod %d = %d\n", row, col, gen,
           possibilities, nextCellID);

  switch (nextCellID) {
  case 0:
    if (up != NULL && preyOrCorpse(up, type))
      return up;
    else if (right != NULL && preyOrCorpse(right, type))
      return right;
    else if (down != NULL && preyOrCorpse(down, type))
      return down;
    else if (left != NULL && preyOrCorpse(left, type))
      return left;
  case 1:
    if (up != NULL && preyOrCorpse(up, type)) {
      if (right != NULL && preyOrCorpse(right, type))
        return right;
      else if (down != NULL && preyOrCorpse(down, type))
        return down;
      else if (left != NULL && preyOrCorpse(left, type))
        return left;
    } else {
      if (right != NULL && preyOrCorpse(right, type)) {
        if (down != NULL && preyOrCorpse(down, type))
          return down;
        else if (left != NULL && preyOrCorpse(left, type))
          return left;
      } else if (left != NULL && preyOrCorpse(left, type))
        return left;
    }
  case 2:
    if (up != NULL && preyOrCorpse(up, type)) {
      if (right != NULL && preyOrCorpse(right, type)) {
        if (down != NULL && preyOrCorpse(down, type))
          return down;
        else if (left != NULL && preyOrCorpse(left, type))
          return left;
      } else if (left != NULL && preyOrCorpse(left, type))
        return left;
    } else if (left != NULL && preyOrCorpse(left, type))
      return left;
  case 3:
    if (left != NULL && preyOrCorpse(left, type))
      return left;
  default:
    return cell;
  }
}

void starvation(world_t *world, creature_t *creature) {
  int row = creature->row;
  int col = creature->col;
  char* fox = "Fox";
  char* species = creature->species;
  cell_t *cell = world->board[row][col];
  if (verbose)
    printf("\nStarvation! %s %d in position (%d,%d) died :(\n",
           creature->species, creature->id, cell->creature->row,
           cell->creature->col);
  creature->alive = false;
  world->creatures--;
  cell->creature = NULL;
  if (strcmp(species, fox) == 0) {
    world->foxes--;
  }
}

void eatCreature(world_t *world, creature_t *predator, char type, char target) {
  if (predator->alive == false || predator->genCreated == world->gen)
    return;
  int row = predator->row;
  int col = predator->col;
  cell_t *current = world->board[row][col];
  cell_t *nextCell = chooseNextPrey(world, current, target);

  if (nextCell == current) {
    predator->food--;
    if (predator->food == 0) {
      starvation(world, predator);
      return;
    }
    movement(world, predator, type);
  } else if (nextCell->type == 'C') {
    creature_t *nextCreature = nextCell->creature;

    int predatorReprAge, nextCreatureReprAge;
    if (predator->species[0] == 'F') {
      predatorReprAge = predator->age % (foxRepr + 1);
      nextCreatureReprAge = nextCreature->age % (foxRepr + 1);
    } else if (predator->species[0] == 'R') {
      predatorReprAge = predator->age % (rabbitRepr + 1);
      nextCreatureReprAge = nextCreature->age % (rabbitRepr + 1);
    }

    if (predator->age < nextCreature->age || (predator->age == nextCreature->age && predator->food < nextCreature->food)) {
      if (verbose)
        printf("%s %d (%d) crashed into %s %d (%d) in position (%d,%d) and died\n",
               predator->species, predator->id, predatorReprAge, nextCreature->species,
               nextCreature->id, nextCreatureReprAge, nextCell->row, nextCell->col);
      tryProcreation(world, current);
      killCreature(world, current);
      return;
    } else {
      if (verbose)
        printf("%s %d (%d) killed %s %d (%d) and moved from (%d,%d) to (%d,%d)\n",
               predator->species, predator->id, predatorReprAge, nextCreature->species,
               nextCreature->id, nextCreatureReprAge, row, col, nextCell->row, nextCell->col);
      killCreature(world, nextCell);
      predator->row = nextCell->row;
      predator->col = nextCell->col;
      predator->food = foxHunger;
      nextCell->type = 'C';
      nextCell->creature = predator;
      tryProcreation(world, current);
      return;
    }
  } else {
    if (verbose)
      printf("%s %d ate %s %d and moved from (%d,%d) to (%d,%d)\n",
             predator->species, predator->id, nextCell->creature->species,
             nextCell->creature->id, row, col, nextCell->row, nextCell->col);
    killCreature(world, nextCell);
    predator->row = nextCell->row;
    predator->col = nextCell->col;
    predator->food = foxHunger;
    nextCell->type = 'C';
    nextCell->creature = predator;
    tryProcreation(world, current);
  }
}

void rabbitMovement(world_t *world, creature_t *rabbit) {
  rabbit->age++;
  rabbit->reprAge++;
  movement(world, rabbit, 'R');
}

void foxMovement(world_t *world, creature_t *fox) {
  fox->age++;
  fox->reprAge++;
  eatCreature(world, fox, 'F', 'R');
}
