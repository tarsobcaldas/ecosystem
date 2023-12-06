#include "board.h"
#include "creature.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

cell_t *initCell(int row, int col, char type) {
  cell_t *cell;
  cell = (cell_t *)malloc(sizeof(cell_t));
  cell->row = row;
  cell->col = col;
  cell->type = type;
  cell->creature = (creature_t *)malloc(sizeof(creature_t));
  return cell;
}

cell_t ***initBoard(int rows, int cols) {
  cell_t ***board;
  board = (cell_t ***)malloc(rows * sizeof(cell_t **));
  for (int row = 0; row < rows; row++) {
    board[row] = (cell_t **)malloc(cols * sizeof(cell_t *));
  }
  for (int row = 0; row < rows; row++)
    for (int col = 0; col < cols; col++)
      board[row][col] = initCell(row, col, NOTHING);
  return board;
}

void cleanCell(cell_t *cell) {
  cell->type = NOTHING;
  cell->creature = NULL;
}

void removeCreatures(list_t *creatureList) {
  node_t *node = creatureList->first;
  creature_t *creature;
  while (node != NULL) {
    creature = node->creature;
    if (creature->previousPosition != NULL || creature->alive == false) {
      cleanCell(creature->previousPosition);
    }
    node = node->next;
  }
}

void updateCreatures(cell_t ***board, list_t *creatureList) {
  node_t *node = creatureList->first;
  cell_t *cell;
  creature_t *creature;
  while (node != NULL) {
    creature = node->creature;
    cell = board[creature->row][creature->col];
    if (creature->previousPosition != NULL || creature->alive == false) {
      cleanCell(creature->previousPosition);
    }
    if (cell->type == 'C') {
      cell->type = creatureList->type;
    }
    node = node->next;
  }
}

world_t *initWorld(world_t *world, int rows, int cols) {
  world = (world_t *)malloc(sizeof(world_t));

  world->rows = rows;
  world->cols = cols;
  world->gen = 0;
  world->rocks = 0;
  world->foxes = 0;
  world->rabbits = 0;
  world->creatures = 0;
  world->rabbitsList = initList('R');
  world->foxesList = initList('F');
  world->board = initBoard(rows, cols);
  world->nextGenBoard = initBoard(rows, cols);
  return world;
}

void populateBoard(world_t *world, int foxes, int rabbits, int rocks) {
  int row, col;
  cell_t *cell, *nextGenCell;
  cell_t ***board = world->board;
  double populationStart, foxesStart, rabbitsStart, rocksStart, populationEnd,
      foxesEnd, rabbitsEnd, rocksEnd, populationTime, foxesTime, rabbitsTime,
      rocksTime;
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
      cell->creature = newCreature(world, board, row, col, cell->type);
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
      cell->creature = newCreature(world, board, row, col, cell->type);
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
    nextGenCell = world->nextGenBoard[row][col];
    if (cell->type == NOTHING) {
      if (verbose)
        printf("Rock in position (%d,%d)\n", row, col);
      cell->type = 'X';
      nextGenCell->type = 'X';
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

void printBoard(world_t *world) {
  FILE *output = stdout;
  if (outputFile != NULL)
    output = fopen(outputFile, "w");
  int row, col;
  if (verbose)
    fprintf(stdout, "Printing board...\n");
  fprintf(output, "\nGeneration: %ld\n\t", world->gen);
  for (col = 0; col < world->cols; col++) {
    fprintf(output, "%3d ", col);
  }
  fprintf(output, "\n\t┌");
  for (col = 0; col < world->cols - 1; col++) {
    fprintf(output, "───┬");
  }
  fprintf(output, "───┐");
  for (row = 0; row < world->rows; row++) {
    fprintf(output, "\n%d\t", row);
    for (col = 0; col < world->cols; col++) {
      fprintf(output, "│ %c ", world->board[row][col]->type);
    }
    fprintf(output, "│");
    if (row < world->rows - 1) {
      fprintf(output, "\n\t├");
      for (col = 0; col < world->cols - 1; col++)
        fprintf(output, "───┼");
      fprintf(output, "───┤");
    }
  }
  fprintf(output, "\n\t└");
  for (col = 0; col < world->cols - 1; col++) {
    fprintf(output, "───┴");
  }
  fprintf(output, "───┘\n\n");
}

void printStatus(world_t *world, FILE *output) {
  printf("Generation: %ld\n", world->gen);
  printf("Rabbits: %d\n", world->rabbits);
  printf("Foxes: %d\n", world->foxes);
  printf("Rocks: %d\n", world->rocks);
  printf("Creatures: %d\n", world->creatures);
  printf("\n");
}

void move(world_t *world, cell_t *cell) {
  creature_t *creature = cell->creature;
  char *fox = "Fox";
  char *rabbit = "Rabbit";
  char *species = creature->species;
  if (strcmp(species, rabbit) == 0) {
    rabbitMovement(world, creature);
  } else if (strcmp(species, fox) == 0) {
    foxMovement(world, creature);
  }
}

void newGeneration(world_t *world) {
  int i, currentRabbits, currentFoxes;
  double generationStart, foxesStart, rabbitsStart, generationEnd, foxesEnd,
      rabbitsEnd, generationTime, foxesTime, rabbitsTime;
  cell_t *cell;
  node_t *node;
  cell_t ***nextGenBoard = world->nextGenBoard;
  creature_t *creature;
  list_t *rabbitsList = world->rabbitsList;
  list_t *foxesList = world->foxesList;
  char *fox = "Fox";
  char *rabbit = "Rabbit";
  char *species;
  world->gen++;
  if (verbose) {
    printf("Starting generation %ld\n", world->gen);
    generationStart = clock();
    printf("Moving rabbits...\n");
    rabbitsStart = clock();
  }
  node = rabbitsList->first;
  currentRabbits = world->rabbits;
  for (i = 0; i < currentRabbits; i++) {
    if (node == NULL)
      break;
    creature = node->creature;
    species = creature->species;
    cell = world->board[creature->row][creature->col];
    if (creature->genCreated != world->gen) {
      move(world, cell);
    }
    node = node->next;
  }
  removeCreatures(rabbitsList);
  if (verbose) {
    rabbitsEnd = clock();
    rabbitsTime = (rabbitsEnd - rabbitsStart) / CLOCKS_PER_SEC;
    printf("done in %lf seconds.\n\n", rabbitsTime);
    printf("Moving foxes...\n");
    foxesStart = clock();
  }
  node = foxesList->first;
  currentFoxes = world->foxes;
  for (i = 0; i < currentFoxes; i++) {
    if (node == NULL)
      break;
    creature = node->creature;
    species = creature->species;
    cell = world->board[creature->row][creature->col];
    if (creature->genCreated != world->gen) {
      move(world, cell);
    }
    node = node->next;
  }
  updateCreatures(nextGenBoard, foxesList);
  if (verbose) {
    foxesEnd = clock();
    foxesTime = (foxesEnd - foxesStart) / CLOCKS_PER_SEC;
    printf("done in %lf seconds.\n\n", foxesTime);
    generationEnd = clock();
    generationTime = (generationEnd - generationStart) / CLOCKS_PER_SEC;
    printf("Generation %ld finished in %lf seconds.\n\n", world->gen,
           generationTime);
  }

  cell_t ***aux = world->board;
  world->board = world->nextGenBoard;
  world->nextGenBoard = aux;

  if (!silent)
    printBoard(world);
  if (verbose)
    printList(world);
  if (rabbitsList->size > world->rabbits)
    cleanList(world->rabbitsList);
  if (foxesList->size > world->foxes)
    cleanList(world->foxesList);
};

world_t *populateFromInput(world_t *world) {
  FILE *input = fopen(inputFile, "r");
  int rows, cols;
  if (input == NULL) {
    printf("Error opening file %s\n", inputFile);
    exit(1);
  }
  if (verbose)
    printf("Reading input file...\n");
  fscanf(input, "%d %d %d %d %d %d %d", &rabbitRepr, &foxRepr, &foxHunger,
         &maxGen, &rows, &cols, &nObjects);
  world = initWorld(world, rows, cols);
  cell_t ***board = world->board;

  int i;
  char objectType[10];
  int row, col;

  if (verbose)
    printf("Creating %d objects...\n", nObjects);
  for (i = 0; i < nObjects; i++) {
    fscanf(input, "%s %d %d", objectType, &row, &col);
    if (verbose && strcmp(objectType, "ROCK") == 0)
      printf("%s in position (%d,%d)\n", objectType, row, col);
    if (strcmp(objectType, "ROCK") == 0) {
      world->board[row][col]->type = 'X';
      world->nextGenBoard[row][col]->type = 'X';
      world->rocks++;
    } else if (strcmp(objectType, "RABBIT") == 0) {
      newCreature(world, board, row, col, 'R');
    } else if (strcmp(objectType, "FOX") == 0) {
      newCreature(world, board, row, col, 'F');
    }
  }
  if (verbose) {
    printf("done.\n\n");
    printf("Number of foxes: %d\n", world->foxes);
    printf("Number of rabbits: %d\n", world->rabbits);
    printf("Board populated successfully!\n\n");
  }

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
  destroyList(world->rabbitsList);
  destroyList(world->foxesList);
  free(world);
}
