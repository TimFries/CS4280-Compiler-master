/*
 * Name:  Tim Fries
 * Assignment:  P3-Compiler
 * 
 * File:  staticSemantics.c
 * Created on Apr 29, 2017, 11:03:42 AM
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "staticSemantics.h"


//Declare the token stack equal to the max number of variables we are allowed
//for this project
Token VarStack[MAX_STACK];


//Function pushes the current variables on the stack
void push(Token tokenInfo){
    int i;
    
    
    if(varCount == MAX_STACK){
        printf("Variable stack is full\n");
        exit(1);
    }
    for(i = stackTop; i < varCount; i++){
        
        if(strcmp(VarStack[i].intst, tokenInfo.intst) == 0){
            printf("%s has already been declared in the current scope on line#: %d\n", tokenInfo.intst, VarStack[i].lineNum);
            exit(1);
        }
    }
    
    
    VarStack[varCount] = tokenInfo;
    varCount++;
    
    //Originally was used to keep track what was being put on the stack and how
    //the stack looked after each push call
    /*printf("Just added to the stack\nBOTTOM\n");
    for(i = 0; i < varCount; i++){
        printf("%s\n",VarStack[i].intst);
    }*/
   
}

//Function pops the variables from the current scope
//This function is only called at the end of the <block> 
//when a stop token was found by the parser
void pop(int stackTop){
    
    
    int i;
    for(i = varCount; i > stackTop; i--){
        varCount--;
        memset(VarStack[i].intst, 0, sizeof(VarStack[i].intst));
    }
    
}

//Looks to see if the variable has already been declared in the current scope
int find(char *word){
    
    int i;
    
    for(i = varCount; i > stackTop-1; i--){
        
        if(strcmp(VarStack[i].intst, word) == 0){
            
            return varCount - 1 - i;
            
        }
    }
    
    return -1;
}

//Looks to see if the variable has been declared at all (local scope or global
//scope).
int verify(char *word){
    int i;
    
    for(i = varCount-1; i > -1; i--){
        if(strcmp(VarStack[i].intst, word) == 0){
            return varCount - 1 - i;
            
        }
    }
    return -1;
}

//defaults the char array(string) of the variables of the stack so they can
//be compaired 
void defaultStack(){
    int i;
    for(i = 0; i < MAX_STACK; i++){
        strcpy(VarStack[i].intst, "");
    }
}