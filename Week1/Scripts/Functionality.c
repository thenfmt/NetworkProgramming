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
    //dynamically allocates memory for the string on the heap to ensure that the return memory address is valid.
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
    {
        printf("Account existed\n");

        //  release the memory that is no longer needed
        free(userName);
    }
}

void SignIn()
{
    char* userName = GetInputString("Enter username: ");
    char* password;
    User* foundUser = SearchByUserName(userName);
    if(foundUser == NULL)
        printf("Cannot find account\n");
    else
    {
        password = GetInputString("Enter user password: ");
        if(strcmp(foundUser->password, password) != 0) 
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
        // release the memory that is no longer needed
        free(password);
    }
    // release the memory that is no longer needed
    free(userName);    
}

void Search()
{
    if(signInCount < 1)
        printf("You are not sign in\n\n");
    else
    {
        char* userName = GetInputString("Enter username to search for: ");
        User* foundUser = SearchByUserName(userName);
        if(foundUser != NULL)
        {
            char* statusInStr = foundUser->status == ACTIVE ? "active" : "blocked";
            printf("Account is %s\n", statusInStr);
        }
        else
            printf("Cannot find account\n");
        
        //  release the memory that is no longer needed
        free(userName);
    }
}

void SignOut()
{
    if(signInCount < 1)
        printf("You are not sign in\n");
    else 
    {
        char* userName = GetInputString("Enter your username: ");
        User* foundUser = SearchByUserName(userName);
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

        //  release the memory that is no longer needed
        free(userName);
    }
}
