/*
 * Init Lab - libinitlab.h
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#ifndef _LIBINITLAB_H
#define _LIBINITLAB_H

#include <sys/types.h>
#include <stdint.h>

/*
 * Init Lab handling
 */
void initLab();
void startQuestion(void (*questionFunc)(void), int questionNum, int quietMode);
void truncateStr(char* destCharArray, char* str, int strLen);
int compareStrings(char *s1, char *s2, int maxLen);
void report(char* reportStr);
void checkQ5Answers(char* q5AnsA, char* q5AnsB, char* q5AnsC, char* q5AnsD,
        char* q5AnsE);

/*
 * Questions
 */

void question1();
void evaluateQuestion2();
void evaluateQuestion3();

#endif