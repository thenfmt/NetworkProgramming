#include <iostream>
#include <string>
using namespace std;

int const ACTIVE = 1;
int const BLOCKED = 0;
int const MAX_ATTEMPTS = 3;

class User
{
    public:
        string userName;
        string passwd;
        int status;
        int attempts;

        User(string userName, string passwd);
        User(string userName, string passwd, int status);
        ~User();
};

class Node
{
    public:
        User* data;
        Node* prev;
        Node* next;

        Node(User* data);
};

// define doubly linked list class
class DoublyLinkedList 
{
    private:
        Node* head;
        Node* tail;

    public:
        DoublyLinkedList() ;

        void Insert(User* user);

        void PrintList();

        User* SearchByUserName(string userName);
};