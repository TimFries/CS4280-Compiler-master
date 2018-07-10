/*
 * Name:  Tim Fries
 * Assignment:  P3
 * 
 * File:  codeGeneration.c
 * Created on May 5, 2017, 1:04:00 PM
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codeGeneration.h"
#include "parser.h"


//Variable declaration 
Token VarStack[MAX_STACK];
char *tmpStack[MAX_STACK];
int stackCount = 0;
int Top = 0;
int level = 0;
int tmpVarNum = 0;
int IDs = 0;





//Function pushes the current variables on the stack and outputs PUSH text
//to the .asm file
void pushCode(Token tokenInfo){
    int i;
    
    
    if(stackCount == MAX_STACK){
        printf("Variable stack is full\n");
        exit(1);
    }
    //Since tokenInfo.intst is a char array the command
    //node->child# will not recognize the child as being NULL
    if(tokenInfo.intst[0] == '\0'){
        
        return;
    }
    for(i = Top; i < stackCount; i++){
        
        if(strcmp(VarStack[i].intst, tokenInfo.intst) == 0){
            printf("%s has already been declared in the current scope on line#: %d\n", tokenInfo.intst, VarStack[i].lineNum);
            exit(1);
        }
    }
    
    
    
 
    VarStack[stackCount] = tokenInfo;
    fprintf(output, "\tPUSH\n");
    stackCount++;

    
    
   
}

//Function pops the variables from the current scope
//This function is only called at the end of the <block> 
//when a stop token was found by the parser
void popCode(int Top){
    
    
    int i;
    
    for(i = stackCount; i > Top; i--){
        
        memset(VarStack[i].intst, 0, sizeof(VarStack[i].intst));
        fprintf(output, "\tPOP\n");
        
    }
    
}

//Looks to see if the variable has already been declared in the current scope
int findCode(char *word){
    
    int i;
    
    for(i = stackCount; i > Top-1; i--){
        
        if(strcmp(VarStack[i].intst, word) == 0){
            
            return stackCount - 1 - i;
            
        }
    }
    
    return -1;
}

//Looks to see if the variable has been declared at all (local scope or global
//scope).
int verifyCode(char *word){
    int i;
    
    for(i = stackCount-1; i > -1; i--){
        if(strcmp(VarStack[i].intst, word) == 0){
            return stackCount - 1 - i;
            
        }
    }
    return -1;
}

//defaults the char array(string) of the variables of the stack so they can
//be compared
static void defaultStack(){
    int i;
    for(i = 0; i < MAX_STACK; i++){
        strcpy(VarStack[i].intst, "");
    }
}

//defaults second stack that is used when out putting the correct stackw/stackr positions
void defaultTmpStack(){
    int i;
    for(i = 0; i < MAX_STACK; i++){
        tmpStack[i] = NULL;
    }
}

//Sets a new stack variable that will be used
char *newVar(){
    
    char *tmpVar = malloc(4);
    sprintf(tmpVar, "V%d", tmpVarNum++);
    tmpStack[tmpVarNum - 1] = tmpVar;
    return tmpVar;
}

//Sets a new Loop variable that will be used
char *newTmp(){
    char *ident = malloc(4);
    sprintf(ident, "I%d", IDs);
    return ident;
}

//Sets a new Out call variable used by BR calls
char *newEND(){
    char *ident = malloc(4);
    sprintf(ident, "E%d", IDs);
    return ident;
}

//Start code gen function that is called by main.c
void startGen(Node *node){
    defaultTmpStack();
    codeGen(node);
}

//Function that recursively goes through the parse tree created by the 
//parse and generated the code for the .asm file
//.asm files should support:
// in/out calls (READ/WRITE)
// expression including () and negation &
// IF statement (couldn't get an if/else system to work correctly
// nested if
// loop
// nested loop
// sequence of statements as long as it is not a multiple IFs in the same 
void codeGen(Node* node){
    int i;
    if(node == NULL){
        return;
    }
    
    //Large if/else statement to determine which not type we are currently in
    //This is important in outputting the .asm code in the correct order
    if(node->nodeType == proNode){
        stackCount = 0;
        
        if(node->child1 != NULL){
            codeGen(node->child1);
        }
        if(node->child2 != NULL){
            codeGen(node->child2);
        }
        fprintf(output, "STOP\n");
        for(i = 0; i < MAX_STACK; i++){
            if(tmpStack[i] != NULL){
                fprintf(output, "%s 0\n", tmpStack[i]);
            }
        }
    }
    else if(node->nodeType == varNode){
        int found;
        Top = stackCount;
        found = findCode(node->tokens.intst);
        if(found == -1 || found > level){
            pushCode(node->tokens);
            level++;
        }
        
        if(node->child1 != NULL){
            
            codeGen(node->child1);
        }
        
    }
    else if(node->nodeType == blockNode){
        level = 0;
        Top = stackCount;
        IDs++;
        
        
        codeGen(node->child1);
        codeGen(node->child2);
        
        
        popCode(Top);
        
        
        
    }
    else if(node->nodeType == mvarNode){
        
        int found;
        if(level > 0){
            found = findCode(node->tokens.intst);
            if(found == -1 || found > level){
                
                pushCode(node->tokens);
                level++;
            }
        }
        if(node->child1 != NULL){
            
            codeGen(node->child1);
        }
        
    }
    else if(node->nodeType == expNode){
        char *tmpVar;
        
        if(node->tokens.tokenType == MULTtk){
            codeGen(node->child2);
            tmpVar = newVar();
            fprintf(output, "\t\tSTORE %s\n", tmpVar);
            codeGen(node->child1);
            fprintf(output, "\t\tMULT %s\n", tmpVar);
        }
        else{
            codeGen(node->child1);
        }
        
    }
    else if(node->nodeType == mNode){
        char *tmpVar;
        if(node->tokens.tokenType == DIVIDEtk){
            codeGen(node->child2);
            tmpVar = newVar();
            fprintf(output, "\t\tSTORE %s\n", tmpVar);
            codeGen(node->child1);
            fprintf(output, "\t\tDIV %s\n", tmpVar);
                  
        }
        else{
            codeGen(node->child1);
        }
        
    }
    else if(node->nodeType == tNode){
        char *tmpVar;
        if(node->tokens.tokenType == SUBtk){
            
            codeGen(node->child2);
            tmpVar = newVar();
            fprintf(output, "\t\tSTORE %s\n", tmpVar);
            codeGen(node->child1);
            fprintf(output, "\t\tSUB %s\n", tmpVar);
        }
        else if(node->tokens.tokenType == ADDtk){
            codeGen(node->child1);
            tmpVar = newVar();  
            fprintf(output, "\t\tSTORE %s\n", tmpVar);
            codeGen(node->child2);
            fprintf(output, "\t\tADD %s\n", tmpVar);
        }
        else{
            codeGen(node->child1);
        }
        
    }
    else if(node->nodeType == fNode){
        if(node->tokens.tokenType == ANDtk){
            codeGen(node->child1);
            fprintf(output, "\t\tMULT -1\n");
        }
        else{
            codeGen(node->child1);
        }
        
    }
    else if(node->nodeType == rNode){
        int verify;
        if(node->tokens.tokenType == NUMtk){
            fprintf(output, "\t\tLOAD %s\n", node->tokens.intst);
        }
        else if(node->tokens.tokenType == IDtk){
            verify = verifyCode(node->tokens.intst);
            fprintf(output, "\t\tSTACKR %d\n", verify);
        }
        else{
            codeGen(node->child1);
        }
    }
    else if(node->nodeType == inNode){
        int verify;
        verify = verifyCode(node->tokens.intst);
        char *tmpVar = newVar();
        
        fprintf(output, "\t\tREAD %s\n", tmpVar);
        fprintf(output, "\t\tLOAD %s\n", tmpVar);
        fprintf(output, "\t\tSTACKW %d\n", verify);
    }
    else if(node->nodeType == outNode){
        codeGen(node->child1);
        char *tmpVar = newVar();
        fprintf(output, "\t\tSTORE %s\n", tmpVar);
        fprintf(output, "\t\tWRITE %s\n", tmpVar);
    }
    else if(node->nodeType == loopNode){
        IDs++;
        
        char *tmpVar = newVar();
        char *begin = newTmp();
        char *end = newEND();
        
        
        fprintf(output, "\t%s:\tNOOP\n",begin);
        codeGen(node->child3);
        fprintf(output, "\t\tSTORE %s\n", tmpVar);
        codeGen(node->child1);
        fprintf(output, "\t\tSUB %s\n", tmpVar);
        
        if(node->child2->tokens.tokenType == EQUALtk){
            fprintf(output, "\tBRNEG\t%s\n", end);
            fprintf(output, "\tBRPOS\t%s\n", end);
            
        }
        else if(node->child2->tokens.tokenType == LESSEQtk ){
            fprintf(output, "\tBRPOS\t%s\n", end);
        }
        else if(node->child2->tokens.tokenType == LESStk){
            fprintf(output, "\tBRZPOS\t%s\n", end);
        }
        else if(node->child2->tokens.tokenType == GREATEQtk){
            fprintf(output, "\tBRNEG\t%s\n", end);
        }
        else if(node->child2->tokens.tokenType == GREATERtk){
            fprintf(output, "\tBRZNEG\t%s\n", end);
        }
        else{
            fprintf(output, "\tBRZERO\t%s\n", end);
            
        }
        codeGen(node->child4);
        fprintf(output, "\tBR\t%s\n", begin);
        fprintf(output, "\t%s: \tNOOP\n", end);
        IDs--;
        
    }
    else if(node->nodeType == ifNode){
        IDs++;
        char *tmpVar, *end;
        codeGen(node->child3);
        tmpVar = newVar();
        fprintf(output, "\t\tSTORE %s\n", tmpVar);
        
        codeGen(node->child1);
        fprintf(output, "\t\tSUB %s\n", tmpVar);
        
        end = newEND();
        
        if(node->child2->tokens.tokenType == EQUALtk){
            fprintf(output, "\tBRNEG\t%s\n", end);
            fprintf(output, "\tBRPOS\t%s\n", end);
            
        }
        else if(node->child2->tokens.tokenType == LESSEQtk ){
            fprintf(output, "\tBRPOS\t%s\n", end);
        }
        else if(node->child2->tokens.tokenType == LESStk){
            fprintf(output, "\tBRZPOS\t%s\n", end);
        }
        else if(node->child2->tokens.tokenType == GREATEQtk){
            fprintf(output, "\tBRNEG\t%s\n", end);
        }
        else if(node->child2->tokens.tokenType == GREATERtk){
            fprintf(output, "\tBRZNEG\t%s\n", end);
        }
        else{
            fprintf(output, "\tBRZERO\t%s\n", end);
            
        }
        codeGen(node->child4);
        fprintf(output, "%s: \tNOOP\n", end);
        IDs--;
        
    }
    else if(node->nodeType == assignNode){
        int verify;
        codeGen(node->child1);
        verify = verifyCode(node->tokens.intst);
        fprintf(output, "\t\tSTACKW %d\n", verify);
        
    }
    else{
        codeGen(node->child1);
        codeGen(node->child2);
        codeGen(node->child3);
        codeGen(node->child4);
    }
    
    
    return;
}
