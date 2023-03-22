#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "DataStructured.h"

void ReadAccountFromFile();
void WriteAccountToFile(User* user);
void UpdateAccountStatusInFile(User* user);

#endif