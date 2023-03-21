#include <iostream>
#include <string>
#include "Functionality.h"

using namespace std;


User* InputUserData()
{
    string userName;
    string passwd;

    cout << "Enter user's informations: " << endl;
    cout << "Enter username: ";
    cin >> userName;
    cout << "Enter userPassowrd: ";
    cin >> passwd;

    return new User(userName, passwd);
}

void Functionality::Register(DoublyLinkedList* doublyLinkedList, FileWriter* fileWriter, string& filePath)
{   
    User* inputUser = InputUserData();
    if(doublyLinkedList->SearchByUserName(inputUser->userName) == nullptr)
    {
        doublyLinkedList->Insert(inputUser);
        fileWriter->WriteAccountToFile(inputUser, filePath);
        cout << "Account registration successful!" << endl;
    }
    else 
    {
        inputUser->~User();
        cout << "Username already exists!" << endl;
    }
}

void Functionality::SignIn(DoublyLinkedList* doublyLinkedList, FileWriter* fileWriter, User** currSignIn, string& filePath)
{
    User* inputUser = InputUserData();
    User* foundUser = doublyLinkedList->SearchByUserName(inputUser->userName);
    if(currSignIn == nullptr)
    {
        cout << "Error: User not found" << endl;
        inputUser->~User();
    }
    else if(foundUser->passwd != inputUser->passwd) 
    {
        cout << "Error: Incorrect password" << endl;
        inputUser->~User();

        foundUser->attempts++;
        if(foundUser->attempts >= MAX_ATTEMPTS)
        {
            foundUser->status = BLOCKED;
            fileWriter->UpdateAccountStatusInFile(foundUser, filePath);
            cout << "Account locked." << endl;
        }
    }
    else
    {
        *currSignIn = foundUser;
        cout << "You are now signed in!" << endl;
    }
}

void Functionality::Search(DoublyLinkedList* doublyLinkedList, User* currSignIn)
{
    string searchName;
    cout << "Enter username to search for: ";
    cin >> searchName;
    
    if(currSignIn == nullptr)
        cout << "You are not signed in!" << endl;
    else
    {
        User* foundUser = doublyLinkedList->SearchByUserName(searchName);
        if(foundUser != nullptr)
        {
            string statusInStr = foundUser->status == ACTIVE ? "Active" : "Blocked";
            cout << "Account found." << endl;
            cout << "Username: " << foundUser->userName << endl;
            cout << "Status: " << statusInStr << endl;
        }
        else
            cout << "Account not found." << endl;
    }
}

void Functionality::SignOut(DoublyLinkedList* doublyLinkedList, User** currSignIn)
{
    string signedInUserName;
    cout << "Enter your username: ";
    cin >> signedInUserName;

    if(currSignIn == nullptr || *currSignIn == nullptr)
        cout << "You are not signed in!" << endl;
    else if((*currSignIn)->userName == signedInUserName)
    {
        cout << "Good bye!" << endl;
        *currSignIn = nullptr;
    }
    else
    {
        User* foundUser = doublyLinkedList->SearchByUserName(signedInUserName);
        if(foundUser != nullptr)
            cout << "Account is not sign in!" << endl;
        else
            cout << "Cannot find account" << endl;
    }
}
