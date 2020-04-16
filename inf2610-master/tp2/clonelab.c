/*
 * Clone Lab - clonelab.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include <stdlib.h>

#include "libclonelab.h"
#include "part1.h"
#include "part2.h"

int main(int argc, char** argv) {
    if (argc == 2) {
        int partNum = atoi(argv[1]);
        switch (partNum) {
            case 1:
                startPart(part1, 1, 1);
                break;
            case 2:
                startPart(part2, 2, 1);
                break;
            default:
                break;
        }
        exit(0);
    }
    initLab();
    startPart(part1, 1, 0);
    startPart(part2, 2, 0);
}