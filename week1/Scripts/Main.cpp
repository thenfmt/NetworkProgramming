#include <iostream>
#include <string>
#include "Functionality.h"

using namespace std;

string filePath = "account.txt";
User* currSignIn = nullptr;

DoublyLinkedList* doublyLinkedList = new DoublyLinkedList();
FileReader* fileReader = new FileReader();
FileWriter* fileWriter = new FileWriter();
Functionality* functionality = new Functionality();

void Menu()
{
    int choice;

    while(true) 
    {
        cout << "\n1. Register"
             << "\n2. Sign in"
             << "\n3. Search"
             << "\n4. Sign out"
             << "\nYour choice (1-4, other to quit): ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                functionality->Register(doublyLinkedList, fileWriter, filePath);
                break;

            case 2:
                functionality->SignIn(doublyLinkedList, fileWriter, &currSignIn, filePath);
                break;

            case 3:
                functionality->Search(doublyLinkedList, currSignIn);
                break;

            case 4:
                functionality->SignOut(doublyLinkedList, &currSignIn);
                break;
            
            default:
                cout << "Goodbye!" << endl;
                return;
        }
    }
}

int main()
{
    fileReader->ReadAccountFromFile(doublyLinkedList, filePath);
    Menu();
    
    return 0;
}