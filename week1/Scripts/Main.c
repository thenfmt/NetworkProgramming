#include <stdio.h>
#include "DataStructured.h"
#include "FileSystem.h"
#include "Functionality.h"

extern LinkedList* userList;
extern int signInCount;

void MenuLoop()
{
    int choice;

    while(true) 
    {
        printf("\n1. Register");
        printf("\n2. Sign in");
        printf("\n3. Search");
        printf("\n4. Sign out");
        printf("\nYour choice (1-4, other to quit): ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                Register();
                break;

            case 2:
                SignIn();
                break;

            case 3:
                Search();
                break;

            case 4:
                SignOut();
                break;
            
            default:
                printf("Goodbye!\n");
                return;
        }
    }
}

int main()
{
    signInCount = 0;
    NewLinkedList();
    ReadAccountFromFile();
    PrintList();
    MenuLoop();
    
    return 0;
}