/*
Author: Juuso von Behr
Student number: 0617328
Date: 02.06.2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure for the list, only includes pointers to a line and to next, in orde to create a linked list
typedef struct list{
    char *line;
    struct list *next;
} LIST;

// Declaring all methods used
LIST *readFile(char[]);
void writeFile(char[], LIST *start);
LIST *freeMemory(LIST *start);

//Main method
int main(int argc, char *argv[]){
    char *inputFileName = NULL;
    char *outputFileName = NULL;
    size_t nameLen = 0;
    LIST *start = NULL;
    
    if(argc == 1){ // No command line arguments, get filename from standard input
        fprintf(stdout, "Give input filename: ");
        getline(&inputFileName, &nameLen, stdin);
        inputFileName[strcspn(inputFileName, "\n")] = 0; //Clear newline from file name, found strcspn function from here: https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
        start = readFile(inputFileName);
        writeFile("", start); // Empty string means printing the result to standard output

    } else if(argc == 2){ // One command line argument, used as filename to read from
        inputFileName = argv[1];
        start = readFile(inputFileName);
        writeFile("", start); // Empty string means printing to stdout

    } else if(argc == 3){ //Two command line arguments, first is read file, second is write file
        if(strcmp(argv[1], argv[2]) == 0){ // If filenames are exactly the same, an error is raised
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        } else{
            inputFileName = argv[1];
            outputFileName = argv[2];
            start = readFile(inputFileName);
            writeFile(outputFileName, start);
        }

    } else{ //Too many arguments
        fprintf(stderr, "usage: reverse <input> <output>\n");
        return 1;
    }

    start = freeMemory(start);
    return 0;
}

// Method for reading the file and putting the lines into a linked list, which is created in a reverse order by setting each line that is read to the beginning of the list.
LIST *readFile(char name[]){
    FILE *file;
    LIST *start = NULL;
    LIST *newLine = NULL;
    char *line = NULL;
    size_t len = 0;

    if((file = fopen(name, "r")) != NULL){

        //The main while loop, where the file is read line by line
        while(getline(&line, &len, file) != -1){ // Getline usage help from here (mostly for the -1 comparison): https://blog.devgenius.io/c-programming-hacks-2-efficiently-reading-a-file-line-by-line-407ab2bd96cc
            char checkNewLine = line[strlen(line)-1];
            newLine = (LIST*)malloc(sizeof(LIST));
            
            if(newLine == NULL){
                fprintf(stderr, "Malloc failed.\n");
                exit(1);
            }

            // Checking if there is a newline after the last line of the file, and below implementation adds it if there is not
            // Noticed this was an issue with the test file (obviously could have just added a newline to the test, but this adds overall robustness)
            if(checkNewLine != '\n'){ 
                newLine->line = (char*)malloc(strlen(line)+2); //Allocate memory for the newline
                if(newLine->line == NULL){
                    fprintf(stderr, "Malloc failed.\n");
                    exit(1);
                }

                //Put the line created above the previous line and add a newline
                strcpy(newLine->line, line);
                strcat(newLine->line, "\n");
            } else{
                //There is a newline after the last line, so one less added to the length of the line and no newline added in the end
                newLine->line = (char*)malloc(strlen(line)+1);
                if(newLine->line == NULL){
                    fprintf(stderr, "Malloc failed.\n");
                    exit(1);
                }
                strcpy(newLine->line, line);
            }
            newLine->next = start;
            start = newLine;
        }
        free(line);
    } else{
        fprintf(stderr, "error: cannot open file '%s'.\n", name);
        exit(1);
    }

    fclose(file);
    return start;
}

//Method for writing the output to a file
void writeFile(char name[], LIST *start){
    FILE *file;
    LIST *ptr = start; //This pointer is used for the writing

    //If empty string is provided write to standard output, otherwise write to the provided file
    if(strlen(name) == 0){
        while(ptr != NULL){
            fprintf(stdout, "%s", ptr->line);
            ptr = ptr->next;
        }
    } else{
        if((file = fopen(name, "w")) != NULL){
            while(ptr != NULL){
                fprintf(file, "%s", ptr->line);
                ptr = ptr->next;
            }
            fclose(file);
        } else{
            fprintf(stderr, "error: cannot open file '%s'\n", name);
            exit(1);
        }
    }
    return;
}

//Method for freeing memory
LIST *freeMemory(LIST *start){
    LIST *ptr = start;
    LIST *next;

    //While loop where memory is freed line by line
    while (ptr != NULL){
        next = ptr->next;
        free(ptr->line);
        free(ptr);
        ptr = next;
    }
    return NULL;
}