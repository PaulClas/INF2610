/*
 * Sync Lab - libsynclab.h
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#ifndef _LIBSYNCLAB_H
#define _LIBSYNCLAB_H

#include "installer/libinstaller.h"

/*
 * Sync Lab handling
 */
void initLab();
void startPart(void (*partFunc)(void*), int partNum, int quietMode);
void truncateStr(char* destCharArray, char* str, int strLen);
int compareStrings(char* s1, char* s2, int maxLen);
void report(char* reportStr);

/*
 * Installer handling
 */

void installerTestInit(struct management_data *md);
void installerGradePermutations();
void installerGradeUnstableMode();
void installerInitManagementData(struct management_data *md);
void installerCleanupManagementData(struct management_data *md);

#endif