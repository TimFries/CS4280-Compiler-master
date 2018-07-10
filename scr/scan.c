/*
 * Name:  Tim Fries
 * Assignment:  P1-Compil
 * 
 * File:  scan.c
 * Created on Mar 9, 2017, 12:50:39 AM
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scan.h" //wouldn't work without declaring it
#include "token.h"



//Set extern variables
int numToken = 0; //set the number of tokens found to zero
int lineNum = 1; //set the starting line number
Token *tokens = NULL; //set the token linked list to NULL


//scanner that will return a token{instance, line #, tokentype}
Token scan(FILE *fp){
 
    //variables needed to run the scanner
    Token token;
    char c;
    int state = 0;
    int nextstate;
    char word[48];
    int index = 0;

    //while loop that will continue till a final state(table index that is >= 1000) in the table is found
    //or if an error is found
    while((c = fgetc(fp)) != EOF){
        
        //was running into the issue of /r char being found when reading from the file
        //so if \r is found it will skip the state table look up all together 
        if(c != '\r'){

            nextstate = Table[state][colCheck(c)];
            
            //Error state found
            if(nextstate < 0){
		printErrorCode(nextstate);
                exit(1);
            }
      
            //Final state found
            if(nextstate >= 1000){
            
                //if the nextstate is final and the char isn't a WS the file will 
                //go back one char place
                if(c != ' ' ){
                    fseek(fp, -1, SEEK_CUR);
                }
                //ID final state found
                if(nextstate == 1000){
                    
                    //Check if the word that was found is a keyword
                    if(findKey(word)){
                   
                        //if the word is a keyword it sets the token type
                        token.tokenType = getKeyword(word);
                    }
                    else{
                        //if word wasn't a keyword it sets the token type to IDtk
                        token.tokenType = IDtk;
                        
                    }
                
                    
                    if(strlen(word) > 8){
                            printf("Scan Error: Length of IDtk/Numtk greater than 8 on line #%d for '%s'\n", lineNum, word);
                            exit(1);
                    }
                    //copy word, linNum to the token struct
                    strcpy(token.intst, word);
                    token.lineNum = lineNum;
                    
                    //if the current char that was checked was a newline char
                    //lineNum will increase by one
                    if(c == '\n')
                        lineNum++;
                
                    //return the token info that was found
                    return token;
                }
                else{
                
                    //Sets the token info before returning the token
                    //if the current char was a newline char the lineNum will
                    //be increased by one
                    strcpy(token.intst, word);
                    token.lineNum = lineNum;
                    token.tokenType = nextstate;
                    word == NULL;
                    
                
                    if(c =='\n')
                        lineNum++;
                
                    return token;
                }
            
            }else{
            
                //If the current word's first char is # this will reset the state
                //and word if a WS or newline char is found
                if(word[0] == '#' && nextstate == 0){
                    state = nextstate;
                    word == NULL;
                    index = 0;
                    if(c == '\n')
                        lineNum++;
                }
                //will continue to append to word until a final state is found
                //will not append a newline char to the word
                else if(!isspace(c)){
                               
                    state = nextstate;
                    word[index] = c;
                    word[index+1] = '\0';
                    index++;
                }         
            }
        }
        
    }
    if(findKey(word)){
        token.tokenType = getKeyword(word);
        strcpy(token.intst, word);
        token.lineNum = lineNum;
        return token;
    }
    token.lineNum = lineNum;
    token.tokenType = EOFtk;
    
    return token;
    
}

//Column check returns the column number that is used for the current char that
//is being checked
int colCheck(char c){
    
    
    if(isalpha(c))
        return 0;
    else if(isdigit(c))
        return 1;
    else if(isspace(c)){
        if(c == '\n'){
            
            return 23;
        }
        else{
            return 22;
        }
                
    }
    else{
        switch(c){
            
            case '>': return 2;
                      break;
            case '<': return 3;
                      break;
            case '=': return 4;
                      break;
            case '!': return 5;
                      break;
            case ':': return 6;
                      break;
            case '+': return 7;
                      break;
            case '-': return 8;
                      break;
            case '*': return 9;
                      break;
            case '/': return 10;
                      break;
            case '&': return 11;
                      break;
            case '%': return 12;
                      break;
            case '.': return 13;
                      break;
            case '(': return 14;
                      break;
            case ')': return 15;
                      break;
            case ',': return 16;
                      break;
            case '{': return 17;
                      break;
            case '}': return 18;
                      break;
            case ';': return 19;
                      break;
            case '[': return 20;
                      break;
            case ']': return 21;
                      break;
            case EOF: return 24;
                      break;
            case '#': return 26;
                      break;
            default:  return 25;
                      break;
            
        }
    }
    
}

//return true or false (1/0) if the word is found to be a keyword
int findKey(char *str) {
    
    
    int i;
    int result = 0; // false
    for (i = 0; i < 10; i++) {

        if (!strcasecmp(keywords[i], str)){
            result = 1;
        }
    }
    return result;

}


//Returns the token type for the certain keyword this is used so
//10 columns aren't needed for the FSA table
TkType getKeyword(char *word){
    
    if(strcasecmp(word, "start") == 0) return STARTtk;
    if(strcasecmp(word, "stop") == 0) return STOPtk;
    if(strcasecmp(word, "decision") == 0) return DECISIONtk;
    if(strcasecmp(word, "while") == 0) return WHILEtk;
    if(strcasecmp(word, "void") == 0) return VOIDtk;
    if(strcasecmp(word, "int") == 0) return INTtk;
    if(strcasecmp(word, "return") == 0) return RETURNtk;
    if(strcasecmp(word, "read") == 0) return READtk;
    if(strcasecmp(word, "print") == 0) return PRINTtk;
    if(strcasecmp(word, "program") == 0) return PROGRAMtk;
    
}

//Will print the current token node from the linked list
void print(Token token) {
    
    
    if (token.tokenType == EOFtk) {
	printf("||||||||||||||||| EOFtk |||||||||||||||||\n");
	return;
    }
        
    printf("%10s \t %10s \t\t Line #%d \n", token.intst, tokenNames[token.tokenType - 1000], token.lineNum);
}

//Prints error depending on error number generated from the FSA table
//if one error is found the program exits so if there are multiple errors in the
//scan only the first error will be shown
void printErrorCode(int errorNum){

    switch(errorNum){

	case -2:
		fprintf(stderr, "Error expected '>' after '>' on line #%d\n", lineNum);
		break;
	case -3:
		fprintf(stderr, "Error expected '<' after '<' on line #%d\n", lineNum);
		break;
	case -4:
		fprintf(stderr, "Error expected '=' before '!' on line #%d\n", lineNum);
		break;
    }

}
