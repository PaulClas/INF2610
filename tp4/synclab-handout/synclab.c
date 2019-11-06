/*
 * Sync Lab - synclab.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include <stdlib.h>
#include <time.h>

#include "libsynclab.h"
#include "installer.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    if (argc == 2) {
        int partNum = atoi(argv[1]);
        switch (partNum) {
            case 1:
                startPart(installer, 1, 1);
                break;
            case -1:
                startPart(installerGradePermutations, 1, 1);
                break;
            case -2:
                startPart(installerGradeUnstableMode, -2, 1);
                break;
            default:
                break;
        }
        exit(0);
    }
    initLab();
    startPart(installer, 1, 0);
}