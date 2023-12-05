#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <getopt.h>
#include "board.h"
#include "creature.h"
#include "list.h"

#define ROWS 10
#define COLS 10
#define FOXES 10 
#define RABBITS 10 
#define ROCKS 20 
#define MAX_GEN 100
#define GEN_FOOD_FOXES 10
#define GEN_PROC_FOXES 5
#define GEN_PROC_RABBITS 4


bool verbose, silent;
int foxHunger, foxRepr, rabbitRepr, nObjects, maxGen;
char* inputFile = NULL; 
char* outputFile = NULL;
FILE* output;

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
    {0, 0, 0, 0}
};

int main(int argc, char** argv) {
  double start, end;
  start = clock();
  int rows, cols, foxes, rabbits, rocks;
  world_t* world = NULL;
  FILE* output = stdout;

  rows = ROWS;
  cols = COLS;
  foxes = FOXES;
  rabbits = RABBITS;
  rocks = ROCKS;
  verbose = false;
  silent = false;
  maxGen = MAX_GEN;
  foxRepr = GEN_PROC_FOXES;
  foxHunger = GEN_FOOD_FOXES;
  rabbitRepr = GEN_PROC_RABBITS;

  int opt;
  while ((opt = getopt_long(argc, argv, "i:o:hf:r:x:vsg:b:", long_options, NULL)) != -1) {
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
    printf("You can't have more creatures than board area!\n");
    exit(1);
  }

  if (verbose && silent) {
    printf("You can't be verbose and silent at the same time!\n");
    exit(1);
  }

  if (outputFile != NULL) {
    output = fopen(outputFile, "w");
    if (output == NULL) {
      printf("Error opening output file.\n");
      exit(1);
    }
  }

  if (inputFile != NULL) {
    world = populateFromInput(world);
    if (!silent)
      printBoard(world);
    if (verbose)
      printList(world);
 } else {
    srand(time NULL);
    populateBoard(world, foxes, rabbits, rocks);
    if (!silent)
      printBoard(world);
    if (verbose)
      printList(world);
  }

  int maxCreatures = (world->rows * world->cols) - world->rocks;
  // while ((world->foxes > 0 && world->rabbits > 0) && world->gen < maxGen && world->creatures < maxCreatures) {

  while (world->gen < maxGen && world->creatures < maxCreatures) {
    newGeneration(world);
  }

  if (world->gen == maxGen)
    printf("Reached last generation.\n");
  else if (world->creatures == maxCreatures)
    printf("World is full.\n");
  // else if (world->foxes == 0)
  //   printf("There are no more foxes.\n");
  // else if (world->rabbits == 0)
  //   printf("There are no more rabbits.\n");

  printf("Simulation ended after %ld generations.\n", world->gen);
  printf("This is the final state of the world:\n");
  printStatus(world, output);
  if (!verbose && !silent) {
    printf("Theses are the final creatures of the board:\n");
    printList(world);
  }
  destroyBoard(world);
  if (outputFile != NULL)
    fclose(output);
  end = clock();
  printf("Run lasted %lf seconds.\n", (end - start) / CLOCKS_PER_SEC);
  exit(0);
}
