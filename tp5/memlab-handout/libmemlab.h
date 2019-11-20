/*
 * Mem Lab - libmemlab.h
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#ifndef _LIBMEMLAB_H
#define _LIBMEMLAB_H

/*
 * Mem Lab handling
 */
void initLab();
void startQuestion(void (*questionFunc)(), void* arg1, void* arg2,
                   void* arg3, void* arg4, int questionNum, int quietMode);
void truncateStr(char* destCharArray, char* str, int strLen);
int compareStrings(char* s1, char* s2, int maxLen);
void report(char* reportStr);

/*
 * Mem Sim
 */

void gradeMemSim();
void gradeMemSimHelperFunctions();

#endif