#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataStructured.h"

LinkedList* userList = NULL;
int signInCount;

User* NewUser(char* userName, char* password)
{
    User* user = (User *)malloc(sizeof(User));
    user->userName = userName;
    user->password = password;
    user->status = ACTIVE;
    user->attempts = 0;
    user->isSignIn = false;

    return user;
}

User* LoadUser(char* userName, char* password, int status)
{
    User* user = (User *)malloc(sizeof(User));
    user->userName = userName;
    user->password = password;
    user->status = status;
    user->attempts = 0;
    user->isSignIn = false;

    return user;
}

Node* NewNode(User* data)
{
    Node* node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void NewLinkedList()
{
    userList = (LinkedList *)malloc(sizeof(LinkedList));
    userList->head = NULL;
    userList->tail = NULL;
}

void InsertList(User* user)
{
    Node* node = NewNode(user);
    
    if (userList->head == NULL) 
    {
        userList->head = node;
        userList->tail = node;
    } 
    else 
    {
        userList->tail->next = node;
        node->prev = userList->tail;
        userList->tail = node;
    }
}

void PrintList()
{
    printf("userName\tpassword\tstatus\t\n");
    Node* curr = userList->head;
    while (curr != NULL) 
    {
        printf( "%s\t\t%s\t\t%d\t\n",
                curr->data->userName,
                curr->data->password,
                curr->data->status);

        curr = curr->next;
    }
}

User* SearchByUserName(char* userName)
{
    Node* curr = userList->head;
    while (curr != NULL) 
    {
        if (strcmp(curr->data->userName, userName) == 0)
            return curr->data;

        curr = curr->next;
    }

    return NULL;
}


