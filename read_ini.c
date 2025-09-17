#include <stdio.h>
#include <string.h>

int main(){
    FILE *fptr;

    // Open a file in read mode
    fptr = fopen("config.ini", "r");

    // Store the content of the file
    char myString[100];

    // delimiter
    const char s[2] = "=";
    char* tok;

    // If the file exist
    if(fptr != NULL) {

    // Read the content and print it
    while(fgets(myString, 100, fptr)) {
        // compare strings
        // are equal when `strcmp` is 0
        if (strcmp(myString,"[Settings]\n") == 0) {
            printf("hello\n");
            continue;
        }
        tok = strtok(myString, s);
        printf("%s ", tok);
        printf("%s", strtok(NULL, s));
        // printf("%s", myString);
    }

    // If the file does not exist
    } else {
    printf("Not able to open the file.");
    }

    // Close the file
    fclose(fptr);
    
    return 0;
}