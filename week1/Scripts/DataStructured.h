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
    int attempts;
    bool isSignIn;
} User;

User* NewUser(char* userName, char* password);
User* LoadUser(char* userName, char* password, int status);


typedef struct Node
{
    User* data;
    struct Node* prev;
    struct Node* next;
} Node;

Node* NewNode(User* data);


typedef struct
{
    Node* head;
    Node* tail;
} LinkedList;

void NewLinkedList();
void InsertList(User* user);
void PrintList();
User* SearchByUserName(char* userName);

#endif