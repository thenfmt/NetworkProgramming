#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "IOsystem.h"

using namespace std;

void FileReader::ReadAccountFromFile(DoublyLinkedList* doublyLinkedList, string& filePath)
{
    ifstream infile(filePath);
    if (!infile) 
    {
        // Handling errors when the file cannot be opened
    }

    string line;
    while (getline(infile, line)) 
    {
        istringstream iss(line);
        
        string userName;
        string passwd;
        int status;

        if (!(iss >> userName >> passwd >> status)) 
        {                
            // Handling errors when the line format is incorrect
        }
        doublyLinkedList->Insert(new User(userName, passwd, status));
    }
}

void FileWriter::WriteAccountToFile(User* user, string& filePath) 
{
    ofstream outfile;
    outfile.open(filePath, ios::app);

    outfile << "\n" << user->userName << " " << user->passwd << " " << user->status;

    outfile.close();
}

void FileWriter::UpdateAccountStatusInFile(User* user, string& filePath)
{
    fstream file(filePath, ios::in | ios::out);
    string username, password, status;

    while (file >> username >> password >> status) 
    {
        if (username == username) 
        {
            file.seekp(-1 * (username.length() + password.length() + status.length() + 2), ios::cur);
            file << username << " " << password << " " << user->status << endl;
            break;
        }
    }
}