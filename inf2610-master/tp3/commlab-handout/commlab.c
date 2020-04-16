/*
 * Comm Lab - commlab.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include <stdlib.h>

#include "libcommlab.h"
#include "puzzle1.h"
#include "puzzle2.h"
#include "puzzle3.h"

int main(int argc, char** argv) {
    if (argc == 2) {
        int puzzleNum = atoi(argv[1]);
        switch (puzzleNum) {
            case 1:
                startPuzzle(puzzle1, 1, 1);
                break;
            case 2:
                startPuzzle(puzzle2, 2, 1);
                break;
            case 3:
                startPuzzle(puzzle3, 3, 1);
                break;
            case -3:
                startPuzzle(gradeP3, 3, 1);
                break;
            default:
                break;
        }
        exit(0);
    }
    initLab();
    startPuzzle(puzzle1, 1, 0);
    startPuzzle(puzzle2, 2, 0);
    startPuzzle(puzzle3, 3, 0);
}