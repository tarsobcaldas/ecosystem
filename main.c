#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <getopt.h>
#include "board.h"
#include "creature.h"
#include "list.h"

#define ROWS 20
#define COLS 20
#define FOXES 50 
#define RABBITS 50 
#define ROCKS 100 
#define MAX_GEN 100


bool verbose, silent;

void display_help() {
    printf("Usage: ecosystem [options]\n");
    printf("    -i <file> --input <file>                \n");
    printf("    -o <file> --output <file>               \n");
    printf("    -h --help                               \n");
    printf("    -f --foxes <num>                        \n");
    printf("    -r --rabbits <num>                      \n");
    printf("    -x --rocks <num>                        \n");
    printf("    -v --verbose                            \n");
    printf("    -s --silent                             \n");
    printf("    -b --board-size <rows>x<cols>           \n");
    printf("    -g --max-gen                            \n");
    printf("    -c --config <file>                      \n");
    exit(0);
}

struct option long_options[] = {
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"help", no_argument, 0, 'h'},
    {"foxes", required_argument, 0, 'f'},
    {"rabbits", required_argument, 0, 'r'},
    {"rocks", required_argument, 0, 'x'},
    {"verbose", no_argument, 0, 'v'},
    {"board-size", required_argument, 0, 'b'},
    {"silent", no_argument, 0, 's'},
    {"max-gen", required_argument, 0, 'g'},
    {"config", required_argument, 0, 'c'},
    {0, 0, 0, 0}
};

int main(int argc, char** argv) {
  double start, end;
  start = clock();
  int rows, cols, foxes, rabbits, rocks, maxGen;
  char* inputFile = NULL; 
  char* outputFile = NULL;
  world_t* world = NULL;

  rows = ROWS;
  cols = COLS;
  foxes = FOXES;
  rabbits = RABBITS;
  rocks = ROCKS;
  verbose = false;
  silent = false;
  maxGen = MAX_GEN;

  int opt;
  while ((opt = getopt_long(argc, argv, "i:o:hf:r:x:vsg:c:b:", long_options, NULL)) != -1) {
    switch (opt) {
      case 'i':
        inputFile = optarg;
        break;
      case 'o':
        outputFile = optarg;
        break;
      case 'h':
        display_help();
      case 'f':
        foxes = atoi(optarg);
        printf("Foxes: %d\n", foxes);
        break;
      case 'r':
        rabbits = atoi(optarg);
        printf("Rabbits: %d\n", rabbits);
        break;
      case 'x':
        rocks = atoi(optarg);
        printf("Rocks: %d\n", rocks);
        break;
      case 'v':
        verbose = true;
        break;
      case 's':
        silent = true;
        break;
      case 'g':
        maxGen = atoi(optarg);
        break;
      case 'b':
        sscanf(optarg, "%dx%d", &rows, &cols);
        printf("Board size: %dx%d\n", rows, cols);
        break;
      default:
        break;
    }
  }

  if (rows*cols < foxes + rabbits + rocks) {
    printf("You can't have more creatures than board size!\n");
    exit(1);
  }

  if (verbose && silent) {
    printf("You can't be verbose and silent at the same time!\n");
    exit(1);
  }

  if (inputFile != NULL) {
    world = readInputFile(world, inputFile);
    if (!silent)
      printBoard(world);
 } else {
    world = initBoard(world, rows, cols);
    srand(time NULL);
    populateBoard(world, foxes, rabbits, rocks);
    if (!silent)
      printBoard(world);
    if (verbose)
      printList(world);
  }

  int maxCreatures = (world->rows * world->cols) - world->rocks;
  while ((world->foxes > 0 && world->rabbits > 0) && world->gen < maxGen && world->creatures < maxCreatures) {
    newGeneration(world);
  }

  if (world->gen == maxGen)
    printf("Reached last generation.\n");
  else if (world->creatures == maxCreatures)
    printf("World is full.\n");
  else if (world->foxes == 0)
    printf("There are no more foxes.\n");
  else if (world->rabbits == 0)
    printf("There are no more rabbits.\n");

  printf("Simulation ended after %ld generations.\n", world->gen);
  printf("This is the final state of the world:\n");
  printStatus(world);
  if (!verbose && !silent) {
    printf("Theses are the final creatures of the board:\n");
    printList(world);
  }
  destroyBoard(world);
  end = clock();
  printf("Run lasted %lf seconds.\n", (end - start) / CLOCKS_PER_SEC);
  exit(0);
}
