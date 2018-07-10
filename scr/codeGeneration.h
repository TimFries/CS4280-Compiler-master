/*
 * Name:  Tim Fries
 * Assignment:  P3
 * 
 * File:  codeGeneration.h
 * Created on May 5, 2017, 12:58:07 PM
 * 
 */

/* 
 * File:   codeGeneration.h
 * Author: Tim
 *
 * Created on May 5, 2017, 12:58 PM
 */

#ifndef CODEGENERATION_H
#define CODEGENERATION_H

#include "parser.h"
#include "token.h"
 //Was 100 but there was a small error because of the way I have push set up
#define MAX_STACK 101

extern FILE *output;

//Function prototypes used in 
void pushCode(Token);
void popCode();
int findCode(char*);
int verifyCode(char*);
void startGen(Node *);
void codeGen(Node *);
void defaultTmpStack();
char *newVar();
char *newTmp();
char *newEND();

#endif /* CODEGENERATION_H */

