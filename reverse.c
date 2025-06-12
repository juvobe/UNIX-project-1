/*
Author: Juuso von Behr
Student number: 0617328
Date: 02.06.2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list{
    char *line;
    struct list *next;
} LIST;

int main(int argc, char *argv[]){
    char line[256];
    
    if(argc == 0){

    } else if(argc == 1){
        FILE *fptr = fopen(argv[0], "r");
        if(fptr != NULL){
            while(getline(line, sizeof(line), fptr)){
                fprintf(stdout, line);
            }
        }
    } else{
        fprintf(stderr, "usage: reverse <input> <output>\n");
    }
}