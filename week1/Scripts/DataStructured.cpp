#include <iostream>
#include <string>
#include "DataStructured.h" // added this line to include the header file

using namespace std;

User::User(string userName, string passwd)
{
    this->userName = userName;
    this->passwd = passwd;
    this->status = ACTIVE;
    this->attempts = 0;
}

User::User(string userName, string passwd, int status)
{
    this->userName = userName;
    this->passwd = passwd;
    this->status = status;
    this->attempts = 0;
}

User::~User()
{
}

Node::Node(User* data)
{
    this->data = data;
    this->prev = nullptr;
    this->next = nullptr;
}

// define doubly linked list class
DoublyLinkedList::DoublyLinkedList()
{
    head = nullptr;
    tail = nullptr;
}

void DoublyLinkedList::Insert(User* user)
{
    Node* newNode = new Node(user);

    if (head == nullptr) 
    {
        head = newNode;
        tail = newNode;
    } 
    else 
    {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

void DoublyLinkedList::PrintList()
{
    cout << "userName\t" << "password\t" << "status\t" << endl;
    Node* curr = head;
    while (curr != nullptr) 
    {
        cout    << curr->data->userName << "\t\t"
                << curr->data->passwd << "\t\t"
                << curr->data->status << "\t" << endl;

        curr = curr->next;
    }
}

User* DoublyLinkedList::SearchByUserName(string userName)
{
    Node* curr = head;
    while (curr != nullptr) 
    {
        if (curr->data->userName == userName)
            return curr->data;

        curr = curr->next;
    }

    return nullptr;
}
