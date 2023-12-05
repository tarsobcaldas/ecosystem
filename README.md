# Ecosystem simulation

This repository implements the simulation in C for an ecosystem with
two types of creatures: rabbits and foxes.

The world is generated in a board (default is 20x20) with the set numbers
of rabbits (defaults to 10), foxes (also defaults to 10) and rocks (defaults to 50) 
and runs it for at most the maximum numbers of generations (default is 100)
or until the board if filled, or one of the species is extinct.

The usage of the program is as follows:

```help
Usage: ecosystem [options]
    -i <file> --input <file>               
    -o <file> --output <file>               
    -h --help                               
    -f --foxes <num>                        
    -r --rabbits <num>                      
    -x --rocks <num>                        
    -v --verbose                            
    -s --silent                             
    -b --board-size <rows>x<cols>           
    -g --max-gen                            
 ```

The creatures move in the following order:
1. Rabbits move.
2. Foxes try to eat a rabbit in one of the adjacents cells, if there isn't any, 
   they just move using the same rule as the rabbits.

When moving, the creatures first tries to go up, then down, then left, then right. If
all the spaces are occupied, it stays in the same place (the fox checks if can eat
a rabbit in the same order). 

If the rabbit have been alive for it's procreation time (default is 5 generations),
it leaves a baby in the spot he was before moving, and the procreation counter
is reset. The same for the fox (also defaults to 5 generations before procreating).

The foxes have to eat a rabbit to get food. If it's food counter (default is 10) 
reaches 0, it dies, and if they are able to eat, the food counter is reset.

## To be done:

[X] Accept an input file properly
[ ] Print to an output file
[X] Be able to accept a config file
[ ] Fix problem with foxes trying to eat the same rabbit
[ ] Develop parallelism with multithreading
[ ] Implement silent and verbose levels for better testing and debbuging.
