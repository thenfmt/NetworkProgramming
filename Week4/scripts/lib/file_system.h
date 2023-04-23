#ifndef __file_system_h__
#define __file_system_h__

#include "data_structured.h"

void ReadAccountFromFile();
void WriteAccountToFile(User* user);
void UpdateAccountStatusInFile(User* user);
void UpdateAccountPassword(User* user);

#endif