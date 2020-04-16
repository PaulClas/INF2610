/*
 * Clone Lab - libclonelab.h
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#ifndef _LIBCLONELAB_H
#define _LIBCLONELAB_H

#include <sys/types.h>

/*
 * Part 2
 */
void registerProc(int procLevel, int procNum, pid_t pid, pid_t ppid);

/*
 * Clone Lab handling
 */
void initLab();
void startPart(void (*partFunc)(void), int partNum, int quietMode);
void truncateStr(char* destCharArray, char* str, int strLen);
int compareStrings(char* s1, char* s2, int maxLen);
void report(char* reportStr);
void checkBufValue(char* procName);

#endif