/*
 * Comm Lab - libcommlab.h
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#ifndef _LIBCOMMLAB_H
#define _LIBCOMMLAB_H

void initLab();
void startPuzzle(void (*puzzleFunc)(void), int puzzleNum, int quietMode);
void truncateStr(char* destCharArray, char* str, int strLen);
int compareStrings(char* s1, char* s2, int maxLen);
void report(char* reportStr);
void gradeP3();

// Pour le puzzle 2
void checkExchangerMessage(char* message);

#endif