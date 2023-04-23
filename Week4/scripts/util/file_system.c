#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structured.h"
#include "file_system.h"

#define FILE_PATH "account.txt"

extern LinkedList* userList;

void ReadAccountFromFile() 
{
    FILE* file = fopen(FILE_PATH, "r");
    if (file == NULL) 
    {
        // Handling errors when the file cannot be opened
        printf("null file");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) 
    {
        //To avoid the pointer pointing to a local address, 
        //we dynamically allocate memory for user variables declared as 'char* userName;' and 'char* passowrd;'.
        char* userName = malloc(sizeof(char) * STRING_LENGTH);
        char* password = malloc(sizeof(char) * STRING_LENGTH);
        int status;

        if (sscanf(line, "%s %s %d", userName, password, &status) != 3) 
        {
            // Handling errors when the line format is incorrect
            continue;
        }

        User* user = LoadUser(userName, password, status);
        InsertList(user);
    }
    fclose(file);
}


void WriteAccountToFile(User* user) 
{
    FILE* file = fopen(FILE_PATH, "a");
    if (file == NULL) 
    {
        // Handling errors when the file cannot be opened
        return;
    }

    fprintf(file, "\n%s %s %d", user->userName, user->password, user->status);

    fclose(file);
}


void UpdateAccountStatusInFile(User* user) 
{
    FILE* file = fopen(FILE_PATH, "r+");
    if (file == NULL) 
    {
        // Handling errors when the file cannot be opened
        return;
    }

    char username[STRING_LENGTH];
    char password[STRING_LENGTH];
    int status;
    int found = 0;
    long int pos = 0;
    while (fscanf(file, "%s %s %d", username, password, &status) == 3) 
    {
        //calculates the position of the current record using ftell()
        pos = ftell(file) - strlen(username) - strlen(password) - 3;
        if (strcmp(username, user->userName) == 0) 
        {
            found = 1;
            break;
        }
    }

    if (found) 
    {
        //seeks to the position of the current record using fseek()
        //overwrites the record with the updated data using fprintf()
        fseek(file, pos, SEEK_SET);
        fprintf(file, "%s %s %d\n", user->userName, user->password, user->status);
    }

    fclose(file);
}

void UpdateAccountPassword(User* user) 
{
    FILE* file = fopen(FILE_PATH, "r+");
    if (file == NULL) 
    {
        // Handling errors when the file cannot be opened
        return;
    }

    FILE* tempfp = fopen("temp.txt", "w");

    // Exit if files couldn't be opened
    if (tempfp == NULL) {
        return;
    }

    char username[STRING_LENGTH];
    char password[STRING_LENGTH];
    char line[STRING_LENGTH];

    int status;

    while (fscanf(file, "%s %s %d", username, password, &status) == 3) 
    {
        if (strcmp(username, user->userName) != 0) 
        {
            fprintf(tempfp, "\n%s %s %d", username, password, status);
        }
    }

    fclose(file);
    fclose(tempfp);

    // Delete original file and rename temporary file to original file
    remove(FILE_PATH);
    rename("temp.txt", FILE_PATH);

    WriteAccountToFile(user);
}
