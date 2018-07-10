/*
 * Name:  Tim Fries
 * Assignment:  P3-Compiler
 * 
 * File:  staticSemantics.h
 * Created on Apr 29, 2017, 10:38:15 AM
 * 
 */

/* 
 * File:   staticSemantics.h
 * Author: Tim
 *
 * Created on April 29, 2017, 10:38 AM
 */

#ifndef STATICSEMANTICS_H
#define STATICSEMANTICS_H

#define MAX_STACK 100
#include "token.h" //Needed because the stack is made from the token struct


//globals used between parser and staticsemantics.c functions
//This is so I don't have to pass these variables each time a function is called
extern int varCount; 
extern int stackTop;

//Function prototypes for static semantics checking and scope checking
void defaultStack();
void push(Token);
void pop();
int find(char*);
int verify(char*);

#endif /* STATICSEMANTICS_H */

