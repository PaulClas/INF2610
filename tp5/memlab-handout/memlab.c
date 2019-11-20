/*
 * Mem Lab - memlab.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "libmemlab.h"
#include "memsim.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    if (argc > 1) {
        int questionNum = atoi(argv[1]);
        switch (questionNum) {
            case 1:
                if (argc > 2 && argv[2] != NULL) {
                    startQuestion(gradeMemSim, initMemoryState, cleanMemoryState,
                              processMemoryRequest, argv[2], 1, 1);
                } else {
                    printf("[memlab] ERROR: You must specify the name of the test file.\n");
                }
                break;
            case -1:
                startQuestion(gradeMemSim, initMemoryState, cleanMemoryState,
                              processMemoryRequest, (void*)0, -1, 1);
                break;
            case -2:
                startQuestion(gradeMemSimHelperFunctions, getPageOffset,
                              getPageNumber, getStartPageAddress,
                              (void*)0, -2, 1);
                break;
            default:
                break;
        }
        exit(0);
    }
    initLab();
    startQuestion(gradeMemSim, initMemoryState, cleanMemoryState,
                  processMemoryRequest, (void*)0, 1, 0);
}