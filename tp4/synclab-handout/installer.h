/*
 * Sync Lab - installer.h
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#ifndef _INSTALLER_H
#define _INSTALLER_H

#include "installer/libinstaller.h"

void installer(void *md);
void installPackage(int packageNum, struct management_data *md);
void postPackageInstall(int packageNum, struct management_data *md);

#endif