/*
 * Name:  Tim Fries
 * Assignment:  P1-Compil
 * 
 * File:  main.c
 * Created on Mar 9, 2017, 12:39:26 AM
 * 
 */

//include files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "codeGeneration.h"

FILE *output;

//start of main
int main(int argc, char *argv[]){

    //Variable declarations
    FILE *fp;
    char *data;
    char buff[255];
    char *outfile;
    char *file;
    Node *node;

    //switch case for arguments
    switch(argc){

        //if no argument is given or < filename is given this will creat a tmp
        //file and take everything the user inputs on the keyboard and puts it 
        //into the file until ctrl d is pressed
	case 1:

            fp = fopen("tmp.txt", "w");

            while(fgets(buff, 255, stdin)){
		data = buff;
		fprintf(fp, data);
            }
            fclose(fp);

            fp = fopen("tmp.txt", "r");
            outfile = malloc(strlen("out.asm"));
            outfile = "out.asm";
            
            break;

        //if an argument is given it will check if the file can be opened
        //if the file can't be opened an error will output to the screen    
	case 2: 
            
            outfile = malloc(strlen(argv[1]) + strlen(".asm") +2);
            strncat(outfile, argv[1], strlen(argv[1]));
            strncat(outfile, ".asm", strlen(".asm"));
            fp = fopen(strcat(argv[1], ".4280E01"), "r");
            
            if(fp == NULL){

		fprintf(stderr, "File not found or can't be opened\n");
                exit(1);
                
            }
            
            break;

	default:
            fprintf(stderr, "ERROR:  Too many arguments\n");
            return 0;
	}

    
    
    output = fopen(outfile, "w");
    //Calls the parser of the compiler
    node = parser(fp);
    //printTree(node, 0);
    startGen(node);
    
    printf("\nASM code generated in file: %s\n", outfile);
    //close the file and exits the program
    fclose(output);
    fclose(fp);
    return 0;

}

