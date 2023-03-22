#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataStructured.h"
#include "Functionality.h"
#include "FileSystem.h"

extern LinkedList* userList;
extern int signInCount;

char* GetInputString(char* message)
{
    char* str = malloc(sizeof(char) * STRING_LENGTH);
    printf("%s\n", message);
    scanf("%s", str);

    return str;
}

void Register()
{   
    char* userName = GetInputString("Enter username: ");
    char* userPassword;
    if(SearchByUserName(userName) == NULL)
    {
        userPassword = GetInputString("Enter user password: ");
        User* user = NewUser(userName, userPassword);
        InsertList(user);
        WriteAccountToFile(user);
        printf("Successful registration \n");
    }
    else 
        printf("Account existed\n");
}

void SignIn()
{
    char* userName = GetInputString("Enter username: ");
    User* foundUser = SearchByUserName(userName);
    if(foundUser == NULL)
    {
        printf("Cannot find account\n");
        return;
    }

    char* userPassword = GetInputString("Enter user password: ");
    if(strcmp(foundUser->password, userPassword) != 0) 
    {
        printf("Password is incorrect\n");

        foundUser->attempts++;
        if(foundUser->attempts >= MAX_ATTEMPTS)
        {
            foundUser->status = BLOCKED;
            UpdateAccountStatusInFile(foundUser);
            printf("Account is blocked\n");
        }
    }
    else
    {
        signInCount++;
        foundUser->isSignIn = true;
        printf("Hello %s \n", foundUser->userName);
    }
}

void Search()
{
    if(signInCount < 1)
        printf("You are not sign in\n\n");
    else
    {
        User* foundUser = SearchByUserName(GetInputString("Enter username to search for: "));
        if(foundUser != NULL)
        {
            char* statusInStr = foundUser->status == ACTIVE ? "active" : "blocked";
            printf("Account is %s\n", statusInStr);
        }
        else
            printf("Cannot find account\n");
    }
}

void SignOut()
{
    if(signInCount < 1)
        printf("You are not sign in\n");
    else 
    {
        User* foundUser = SearchByUserName(GetInputString("Enter your username: "));
        if(foundUser == NULL)
            printf("Cannot find account\n");
        else if(foundUser->isSignIn)
        {
            foundUser->isSignIn = false;
            signInCount--;
            printf("Goodbye %s\n", foundUser->userName);
        }
        else
            printf("Account is not sign in\n");
    }
}
