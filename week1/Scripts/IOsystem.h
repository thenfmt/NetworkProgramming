#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "DataStructured.h"

using namespace std;

class FileReader
{
    public:
        void ReadAccountFromFile(DoublyLinkedList* doublyLinkedList, string& filePath); 
};

class FileWriter
{
    public:
        void WriteAccountToFile(User* user, string& filePath);
        void UpdateAccountStatusInFile(User* user, string& filePath);
};