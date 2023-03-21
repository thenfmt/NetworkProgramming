#include <iostream>
#include <string>
#include "IOsystem.h"

using namespace std;

class Functionality
{
    public:
        void Register(DoublyLinkedList* doublyLinkedList, FileWriter* fileWriter, string& filePath);
        void SignIn(DoublyLinkedList* doublyLinkedList, FileWriter* filewriter, User** currSignIn, string& filePath);
        void Search(DoublyLinkedList* doublyLinkedList, User* currSignIn);
        void SignOut(DoublyLinkedList* doublyLinkedList, User** currSignIn);
};