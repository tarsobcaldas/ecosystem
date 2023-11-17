#include "board.h"
#include "creature.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NOTHING ' '

extern bool verbose, silent;


cell_t *initCell(world_t *world, int row, int col, char type) {
  cell_t *cell;
  cell = (cell_t *)malloc(sizeof(cell_t));
  cell->row = row;
  cell->col = col;
  cell->type = type;
  cell->creature = (creature_t*)malloc(sizeof(creature_t));
  return cell;
}

world_t *initBoard(world_t *world, int rows, int cols) {
  world = (world_t *)malloc(sizeof(world_t));

  int row, col;
  world->rows = rows;
  world->cols = cols;
  world->gen = 0;
  world->foxes = 0;
  world->rabbits = 0;
  world->rocks = 0;
  world->creatures = 0;
  world->creaturesList = initList(world);
  world->board = (cell_t ***)malloc(rows * sizeof(cell_t **));
  for (row = 0; row < rows; row++) {
    world->board[row] = (cell_t **)malloc(cols * sizeof(cell_t *));
  }

  for (row = 0; row < rows; row++)
    for (col = 0; col < cols; col++)
      world->board[row][col] = initCell(world, row, col, NOTHING);

  return world;
}

void populateBoard(world_t *world, int foxes, int rabbits, int rocks) {
  int row, col;
  cell_t *cell;
  double populationStart, foxesStart, rabbitsStart, rocksStart,
         populationEnd, foxesEnd, rabbitsEnd,  rocksEnd,
         populationTime, foxesTime, rabbitsTime, rocksTime;
  if (verbose) {
    printf("Populating board...\n");
    populationStart = clock();
    printf("Creating %d rabbits:\n", foxes);
    rabbitsStart = clock();
  }
  for (int i = 0; i < rabbits; i++) {
    row = rand() % world->rows;
    col = rand() % world->cols;
    cell = world->board[row][col];
    if (cell->type == NOTHING) {
      cell->type = 'R';
      cell->creature = newCreature(world, row, col, cell->type);
    } else {
      i--;
    }
  }
  if (verbose) {
    rabbitsEnd = clock();
    rabbitsTime = (rabbitsEnd - rabbitsStart) / CLOCKS_PER_SEC;
    printf("done in %lf seconds.\n\n", foxesTime);
    printf("Creating %d foxes:\n", rabbits);
    foxesStart = clock();
  }
  for (int i = 0; i < foxes; i++) {
    row = rand() % world->rows;
    col = rand() % world->cols;
    cell = world->board[row][col];
    if (cell->type == NOTHING) {
      cell->type = 'F';
      cell->creature = newCreature(world, row, col, cell->type);
    } else {
      i--;
    }
  }
  if (verbose) {
    foxesEnd = clock();
    foxesTime = (foxesEnd - foxesStart) / CLOCKS_PER_SEC;
    printf("done in %lf seconds.\n\n", rabbitsTime);
    printf("Creating %d rocks:\n", rocks);
    rocksStart = clock();
  }
  for (int i = 0; i < rocks; i++) {
    row = rand() % world->rows;
    col = rand() % world->cols;
    cell = world->board[row][col];
    if (cell->type == NOTHING) {
      if (verbose)
        printf("Rock in position (%d,%d)\n", row, col);
      cell->type = 'X';
      world->rocks++;
    } else {
      i--;
    }
  }
  if (verbose) {
    rocksEnd = clock();
    rocksTime = (rocksEnd - rocksStart) / CLOCKS_PER_SEC;
    printf("done in %lf seconds.\n\n", rocksTime);
    populationEnd = clock();
    populationTime = (populationEnd - populationStart) / CLOCKS_PER_SEC;
    printf("Board populated successfully in %f seconds!\n\n", populationTime);
  }
}

// Print the board in a nice grid with delimiters so it's easy
// to see the position of each creature
void printBoard(world_t *world) {
  int row, col;
  if (verbose)
    printf("Printing board...\n");
  printf("Generation: %ld\n", world->gen);
  if (verbose) {
    printf("\t");
    for (col = 0; col < world->cols; col++) {
      printf("%3d ", col);
    }
    printf("\n");
  }
  if (verbose)
    printf("\t");
  printf("┌");
  for (col = 0; col < world->cols-1; col++) {
    printf("───┬");
  }
  printf("───┐");
  for (row = 0; row < world->rows; row++) {
    printf("\n");
    if (verbose) {
        printf("%d\t", row);
    }
    for (col = 0; col < world->cols; col++) {
      printf("│ %c ",world->board[row][col]->type);
    }
    printf("│");
    if (row < world->rows-1) { 
      printf("\n");
      if (verbose)
        printf("\t");
      printf("├");
      for (col = 0; col < world->cols-1; col++) 
        printf("───┼");
      printf("───┤");
    }
  }
  printf("\n");
  if (verbose)
    printf("\t");
  printf("└");
  for (col = 0; col < world->cols-1; col++) {
    printf("───┴");
  }
  printf("───┘\n");
}

void printStatus(world_t *world) {
  printf("Generation: %ld\n", world->gen);
  printf("Rabbits: %d\n", world->rabbits);
  printf("Foxes: %d\n", world->foxes);
  printf("Rocks: %d\n", world->rocks);
  printf("Creatures: %d\n", world->creatures);
  printf("\n");
}

void move(world_t *world, cell_t *cell) {
  creature_t *creature = cell->creature;
  if (creature->genCreated == world->gen)
    return;
  creature->age++;
  if (cell->type == 'R') {
    rabbitMovement(world, creature);
  } else if (cell->type == 'F') {
    foxMovement(world, creature);
  }
}

void newGeneration(world_t *world) {
  int i;
  double generationStart, foxesStart, rabbitsStart,
         generationEnd, foxesEnd, rabbitsEnd,
         generationTime, foxesTime, rabbitsTime;
  cell_t *cell;
  node_t *node;
  node_t *nextNode;
  creature_t* creature;
  creature_t* nextCreature; 
  list_t *creaturesList = world->creaturesList;
  world->gen++;
  if (verbose) {
    printf("Starting generation %ld\n", world->gen);
    generationStart = clock();
    printf("Moving rabbits...\n");
    rabbitsStart = clock();
  }
  node = creaturesList->first;
  nextNode = node->next;
  for (i = 0; i < world->creatures; i++) {
    if (node == NULL)
      break;
    creature = node->creature;
    cell = world->board[creature->row][creature->col];
    if (cell->type == 'R') {
      move(world, cell);
    }
    node = nextNode; 
    if (nextNode->next != NULL)
    nextNode = nextNode->next;
  }
  if (verbose) {
    rabbitsEnd = clock();
    rabbitsTime = (rabbitsEnd - rabbitsStart) / CLOCKS_PER_SEC;
    printf("done in %lf seconds.\n\n", rabbitsTime);
    printf("Moving foxes...\n");
    foxesStart = clock();
  }
  node = creaturesList->first;
  nextNode = node->next;
  for (i = 0; i < world->creatures; i++) {
    if (node == NULL)
      break;
    creature = node->creature;
    cell = world->board[creature->row][creature->col];
    if (cell->type == 'F') {
      move(world, cell);
    }
    node = nextNode; 
    if (nextNode->next != NULL)
      nextNode = nextNode->next;
  }
  if (verbose) {
    foxesEnd = clock();
    foxesTime = (foxesEnd - foxesStart) / CLOCKS_PER_SEC;
    printf("done in %lf seconds.\n\n", foxesTime);
    generationEnd = clock();
    generationTime = (generationEnd - generationStart) / CLOCKS_PER_SEC;
    printf("Generation %ld finished in %lf seconds.\n\n", world->gen, generationTime);
  }
  if (!silent) 
    printBoard(world);
  if (verbose)
    printList(world);
  if (creaturesList->size > world->creatures)
    cleanList(world);
};

void readBoard(world_t *world, FILE *input) {
  char c;
  int i = 0, j = 0;
  while ((c = fgetc(input)) != EOF) {
    if (c == '\n') {
      i++;
      j = 0;
    } else {
      world->board[i][j]->type = c;
      j++;
    }
  }
  for (i = 0; i < world->rows; i++) {
    for (j = 0; j < world->cols; j++) {
      if (world->board[i][j]->type == 'F') {
        newCreature(world, i, j, 'F');
      } else if (world->board[i][j]->type == 'R') {
        newCreature(world, i, j, 'R');
      } else if (world->board[i][j]->type == 'X') {
      }
    }
  }
}

world_t *readInputFile(world_t *world, char *filename) {
  FILE *input = fopen(filename, "r");
  int rows, cols;
  fscanf(input, "%d %d", &rows, &cols);
  world = initBoard(world, rows, cols);
  readBoard(world, input);
  fclose(input);
  return world;
}

void destroyBoard(world_t *world) {
  for (int i = 0; i < world->rows; i++) {
    for (int j = 0; j < world->cols; j++) {
      free(world->board[i][j]->creature);
      free(world->board[i][j]);
    }
    free(world->board[i]);
  }
  free(world->board);
  destroyList(world);
  free(world);
}
