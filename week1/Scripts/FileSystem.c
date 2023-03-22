#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataStructured.h"
#include "FileSystem.h"

#define FILE_PATH "account.txt"

extern LinkedList* userList;

void ReadAccountFromFile() {
    FILE* file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        // Handling errors when the file cannot be opened
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char* userName = malloc(sizeof(char) * STRING_LENGTH);
        char* password = malloc(sizeof(char) * STRING_LENGTH);
        int status;

        if (sscanf(line, "%s %s %d", userName, password, &status) != 3) {
            // Handling errors when the line format is incorrect
            continue;
        }
        User* user = LoadUser(userName, password, status);
        InsertList(user);
    }
    fclose(file);
}


void WriteAccountToFile(User* user) {
    FILE* file = fopen(FILE_PATH, "a");
    if (file == NULL) {
        // Handling errors when the file cannot be opened
        return;
    }

    fprintf(file, "\n%s %s %d", user->userName, user->password, user->status);

    fclose(file);
}


void UpdateAccountStatusInFile(User* user) {
    FILE* file = fopen(FILE_PATH, "r+");
    if (file == NULL) {
        // Handling errors when the file cannot be opened
        return;
    }

    char username[STRING_LENGTH];
    char password[STRING_LENGTH];
    int status;
    int found = 0;
    long int pos = 0;
    while (fscanf(file, "%s %s %d", username, password, &status) == 3) {
        pos = ftell(file) - strlen(username) - strlen(password) - 3;
        // file.seekp(-1 * (username.length() + password.length() + status.length() + 2), ios::cur);
        if (strcmp(username, user->userName) == 0) {
            found = 1;
            break;
        }
    }
    if (found) {
        fseek(file, pos, SEEK_SET);
        fprintf(file, "%s %s %d\n", user->userName, user->password, user->status);
    }

    fclose(file);
}