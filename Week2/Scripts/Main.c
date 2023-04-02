#include <stdio.h>
#include <stdlib.h>

#include "Functionality.h"

int main(int argc, char *argv[]) 
{

    if(IsIPAddress(argv[1]))
        PrintInfoByIPAddress(argv[1]);
    else if(IsDomain(argv[1]))
        PrintInfoByHostName(argv[1]);
    else
        printf("Not found information\n");
        
    return 0;
}
