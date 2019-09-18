/*
 * Init Lab - initlab.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include <stdlib.h>
#include <time.h>

#include "libinitlab.h"
#include "q2/libq2.h"
#include "q3/libq3.h"
#include "q4/libq4.h"
#include "answers-q5.h"

int main(int argc, char** argv) {
    srand(time(NULL));
    
    if (argc == 2) {
        int questionNum = atoi(argv[1]);
        switch (questionNum) {
            case 1:
                startQuestion(question1, 1, 1);
                break;
            case 2:
                startQuestion(evaluateQuestion2, 2, 1);
                break;
            case 3:
                startQuestion(evaluateQuestion3, 3, 1);
                break;
            case 4:
                startQuestion(evaluateQuestion4, 4, 1);
                break;
            case 0:
                checkQ5Answers(
                    Q5_ANS_A,
                    Q5_ANS_B,
                    Q5_ANS_C,
                    Q5_ANS_D,
                    Q5_ANS_E
                );
                break;
            default:
                break;
        }
        exit(0);
    }
    initLab();
    startQuestion(question1, 1, 0);
    startQuestion(evaluateQuestion2, 2, 0);
    startQuestion(evaluateQuestion3, 3, 0);
    startQuestion(evaluateQuestion4, 4, 0);
    checkQ5Answers(
        Q5_ANS_A,
        Q5_ANS_B,
        Q5_ANS_C,
        Q5_ANS_D,
        Q5_ANS_E
    );
}