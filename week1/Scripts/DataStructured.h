#ifndef __DATASTRUCTURED_H__
#define __DATASTRUCTURED_H__

#include <stdbool.h>

#define ACTIVE 1
#define BLOCKED 0
#define MAX_ATTEMPTS 3
#define STRING_LENGTH 50

typedef struct User
{
    char* userName;
    char* password;
    int status;

    // These fields serve a functional purpose and will not be saved in the data file.
    int attempts;
    bool isSignIn;
} User;

typedef struct Node
{
    User* data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct
{
    Node* head;
    Node* tail;
} LinkedList;

User* NewUser(char* userName, char* password);
User* LoadUser(char* userName, char* password, int status);

Node* NewNode(User* data);

void NewLinkedList();
void InsertList(User* user);
User* SearchByUserName(char* userName);
// void PrintList();    This function is meant to verify the reading process but is no longer necessary.

#endif