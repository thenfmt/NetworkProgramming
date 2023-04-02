#ifndef __FUNCTIONALITY_H__
#define __FUNCTIONALITY_H__

#include <stdbool.h>

bool IsIPAddress(char* str);
bool IsDomain(char* str);
void PrintInfoByHostName(char* hostName);
void PrintInfoByIPAddress(char* ipAddress);

#endif