//All include files needed to make P2 run correctly
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scan.h"
#include "parser.h"
#include "staticSemantics.h"
#include "token.h"


/* While parser is being used and BNF syntax is being run, the static variable 
   semantics is also being run along side it.  This is so even if it passes
   the BNF syntax the parse won't be okay if a static semantics error was found
   along the way.*/


/* STATIC SEMANTICS functions are called in the vars, mvars, block, and R functions
   the scope count gets changed when entering the block, because I picked to
   do local variables*/
//Global variables used in parser
Token token = {"NA", 0, UNKNOWNtk};
int varCount = 0;
int stackTop = 0;
int count = 0;

FILE *fp;

//This function is the main parser function
//Some things will only be stated here regarding the other function in parser.c
//Because I rather not restate certain things
Node *parser(FILE *filename){
    
    //Defaults the variables and rewinds the fp
    //I think it should work without the rewind(fp)
    //because I was still calling scanner in the main.c
    lineNum = 1;
    fp = filename;
    rewind(fp);
    defaultStack();
    //The first token is scanned from the file and placed in the token struct
    //Then it checks if the token that was found was the PROGRAMtk or EOFtk
    //If it is neither of the two tokens it sets an error and exits
    //Loops through the project BNF functions to generate the parse tree that
    //will be printed
    token = scan(fp);
    Node *root = NULL;
    
    if(token.tokenType == PROGRAMtk){
        token = scan(fp);
        root = program();
        
    }	
    else{
        
        if(token.tokenType == EOFtk){
            printf("Error: file was empty\n");
        }
        else{
            printf("Error: expected PROGRAMtk, instead of %s on line #%d\n", token.intst, token.lineNum);
        }
        exit(1);
    }
    
    if(token.tokenType == EOFtk){
        printf("Parse is OK\n");
    }
    else{
        printf("Error: expected EOFtk, instead of %s on line #%d\n", token.intst, token.lineNum);
        exit(1);
    }
    printf("Variable Semantics OKAY\n");
    
    
    //printTree(root, 0);
    //Calls the print function to print the parse tree
    
    
    
    
    return root;
    
}

Node *program(){
    
    Node *node = getNode(proNode);
    
    node->child1 = vars();
    node->child2 = block();
    return node;
    
}

Node *vars(){
    
    Node *node = getNode(varNode);
    int found; 
    stackTop = varCount;
    if(token.tokenType == INTtk){
        token = scan(fp);
        
        if(token.tokenType == IDtk){
            found = find(token.intst); //Checks to see if the variable is found in the current scope
                                       //if it isn't the BNF continues and the variable is pushed on the stack
                                       //if it is the error is outputted and the program exits
            if(found == -1 || found > count){
                push(token);
                count += 1;
                node->tokens = token;
            
                token = scan(fp);
            }    
            else if(found < count){
                printf("'%s' has already been defined in the current scope.\n", token.intst);
                exit(1);
            }           
            
            
            
        }
        else{
            printf("Error: expected IDtk, instead of %s on line #%d\n", token.intst, token.lineNum);
            exit(1);
        }
        node->child1 = mvars();
        return node;
    }
    else{
        
        return node;
    }
}

Node *mvars(){
    
    Node *node = getNode(mvarNode);
    int found;
    if(token.tokenType == COLONtk){
        
        token = scan(fp);
        
        if(token.tokenType == IDtk){
            
            if(count > 0){
                found = find(token.intst);//Checks to see if the variable is found in the current scope
                                          //if it isn't the BNF continues and the variable is pushed on the stack
                                          //if it is the error is outputted and the program exits
                if(found == -1 || found > count){
                    push(token);
                    count += 1;
                    node->tokens = token;
                    token = scan(fp);
                    node->child1 = mvars();
                }
                else if(found < count){
                    printf("'%s' has already been defined in the current scope.\n", token.intst);
                    exit(1);
                }
            }
            
            
        }
        else{
            
            printf("Error: expected IDtk, instead of %s on line #%d\n", token.intst, token.lineNum);
            exit(1);
        }
        
        
        return node;
    }
    else{
        return node;
    }
}

Node *block(){
    
    Node *node = getNode(blockNode);
    count = 0; //changes the current count to 0 because we entered a new scope
    stackTop = varCount; //changes the current top of the stack position 
    if(token.tokenType == STARTtk){
        
        token = scan(fp);
        node->child1 = vars();
        
        //if(token.tokenType == COLONtk){
        
        node->child2 = stats();
        
        
        if(token.tokenType == STOPtk){
            pop(stackTop); //stop means that it is the end of the block/scope and
                           //current scope variables must be popped from the stack
            token = scan(fp);
            return node;
        }
        else{
            printf("Error: expected STOPtk, instead of %s on line #%d\n", token.intst, token.lineNum);
            exit(1);
        }
        //}
        //else{
        //	printf("Error: expected COLONtk, instead of %s on line #%d\n", token.intst, token.lineNum);
        //	exit(1);
        //}
    }
    else{
        printf("Error: expected STARTtk, instead of %s on line #%d\n", token.intst, token.lineNum);
        exit(1);
    }
}

Node *stats(){
    
    Node *node = getNode(statsNode);
    node->child1 = stat();
    node->child2 = mStat();
    return node;
}

Node *stat(){
    
    Node *node = getNode(statNode);
    
    if(token.tokenType == READtk){
        node->child1 = in();
        return node;
    }
    else if(token.tokenType == PRINTtk){
        node->child1 = out();
        return node;
    }
    else if(token.tokenType == STARTtk){
        node->child1 = block();
        return node;
    }
    else if(token.tokenType == DECISIONtk){
        node->child1 = ifTK();
        return node;
    }
    else if(token.tokenType == WHILEtk){
        node->child1 = loop();
        return node;
    }
    else if(token.tokenType == IDtk){
        node->child1 = assign();
        return node;
    }
    else{
        
        printf("Error expected READtk, PRINTtk, STARTtk, DECISIONtk, WHILEtk, or IDtk\n");
        printf("instead of %s on line #%d\n", token.intst, token.lineNum);
        exit(1);
    }
}

Node *mStat(){
    
    Node *node = getNode(mstatNode);
    
    if(token.tokenType == READtk || token.tokenType == PRINTtk || token.tokenType == STARTtk || token.tokenType == DECISIONtk
	    || token.tokenType == WHILEtk || token.tokenType == IDtk){
        
        node->child1 = stat();
        node->child2 = mStat();
        return node;
    }
    else{
        return node;
    }
}

Node *in(){
    
    Node *node = getNode(inNode);
    
    if(token.tokenType == READtk){
        
        token = scan(fp);
        if(token.tokenType == GREATERtk){
            
            
            token = scan(fp);
            if(token.tokenType == IDtk){
                int exists = verify(token.intst);
                if(exists == -1){
                    printf("Variable '%s' on line#: %d was not declared in the current scope\n", token.intst, token.lineNum);
                    exit(1);
                }
                node->tokens = token;
                token = scan(fp);
                if(token.tokenType == SEMICOLONtk){
                    
                    token = scan(fp);
                    return node;
                }
                else{
                    printf("Error expected SEMICOLONtk instead of %s on line #%d\n", token.intst, token.lineNum);
                    exit(1);
                }
            }
            else{
                printf("Error expected IDtk instead of %s on line #%d\n", token.intst, token.lineNum);
                exit(1);
            }
        }
        else{
            printf("Error expected GREATERtk instead of %s on line #%d\n", token.intst, token.lineNum);
            exit(1);
        }
    }
    else{
        printf("Error expected READtk instead of %s on line #%d\n", token.intst, token.lineNum);
        exit(1);
    }
}

Node *out(){
    
    Node *node = getNode(outNode);
    
    if(token.tokenType == PRINTtk){
        
        token = scan(fp);
        if(token.tokenType == LESStk){
            
            token = scan(fp);
            node->child1 = expr();
            return node;
        }
        else{
            printf("Error expected LESStk instead of %s on line #%d\n", token.intst, token.lineNum);
            exit(1);
        }
    }
    else{
        printf("Error expected PRINTtk instead of %s on line #%d\n", token.intst, token.lineNum);
        exit(1);
    }
}

Node *expr(){
    
    Node *node = getNode(expNode);
    
    node->child1 = M();
    
    if(token.tokenType == MULTtk){
        node->tokens = token;
        token = scan(fp);
        node->child2  = expr();
        return node;
    }
    else{
        return node;
    }
}

Node *M(){
    
    Node *node = getNode(mNode);	
    
    node->child1 = T();
    
    if(token.tokenType == DIVIDEtk){
        node->tokens = token;
        token = scan(fp);
        node->child2 = M();
        return node;
    }
    else{
        
        return node;
    }
}

Node *T(){
    
    Node *node = getNode(tNode);
    
    node->child1 = F();
    
    if(token.tokenType == ADDtk){
        node->tokens = token;
        token = scan(fp);
        node->child2 = T();
        return node;
    }
    else if(token.tokenType == SUBtk){
        node->tokens = token;
        token = scan(fp);
        node->child2 = T();
        return node;
    }
    else{
        
        return node;
    }
}

Node *F(){
    
    Node *node = getNode(fNode);
    
    if(token.tokenType == ANDtk){
        
        node->tokens = token;
        token = scan(fp);
        node->child1 = F();
        return node;
    }
    else{
	
        node->child1 = R();
        return node;
    }
}

Node *R(){
    
    Node *node = getNode(rNode);
    
    if(token.tokenType == LEFTPAtk){
        
        
        token = scan(fp);
        node->child1 = expr();
        if(token.tokenType == RIGHTPAtk){
            
            
            token = scan(fp);
            return node;
        }
        else{
            
            printf("Error expected RIGHTPAtk instead of %s on line #%d\n", token.intst, token.lineNum);
            exit(1);
        }
    }
    else if(token.tokenType == IDtk){
        int exists = verify(token.intst); //checks to see if the the id that is being used(not declared) is on the
                                          //stack or in the current scope
        if(exists == -1){
            printf("Variable '%s' on line#: %d was not declared in the current scope\n", token.intst, token.lineNum);
            exit(1);
        }
        node->tokens = token;
        token = scan(fp);
        return node;
    }
    else if(token.tokenType == NUMtk){
        
        node->tokens = token;
        token = scan(fp);
        return node;
    }
    else{
        
        printf("Error expected LEFTPARtk, IDtk, or NUMtk instead of %s on line #%d\n", token.intst, token.lineNum);
        exit(1);
    }
}

Node *ifTK(){
    
    Node *node = getNode(ifNode);
    
    if(token.tokenType == DECISIONtk){
        
        token = scan(fp);
        if(token.tokenType == LEFTPAtk){
            
            token = scan(fp);
            node->child1 = expr();
            node->child2 = RO();
            node->child3 = expr();
            
            if(token.tokenType == RIGHTPAtk){
                
                token = scan(fp);
                node->child4  = block();
                return node;
            }
            else{
                printf("Error expected RIGHTPAtk instead of %s on line #%d\n", token.intst, token.lineNum);
                exit(1);
            }
        }
        else{
            printf("Error expected LEFTPAtk instead of %s on line #%d\n", token.intst, token.lineNum);
            exit(1);
        }
    }
    else{
        
        printf("Error expected DECISIONtk instead of %s on line #%d\n", token.intst, token.lineNum);
        exit(1);
    }
}

Node *RO(){
    
    Node *node = getNode(roNode);
    
    if(token.tokenType == LESStk){
        
        node->tokens = token;
	token = scan(fp);
	    if(token.tokenType == ASSIGNtk){
		node->tokens = token;
		node->tokens.tokenType = LESSEQtk;
		token = scan(fp);
		return node;
  
            }
	    else{
		return node;
	    }
    }
    else if(token.tokenType == GREATERtk){
        
        node->tokens = token;
        token = scan(fp);
	
	    if(token.tokenType == ASSIGNtk){
		node->tokens = token;
		node->tokens.tokenType = GREATEQtk;
		token = scan(fp);
		return node;
	    }
	    else{
		return node;
	    }
    }
    else if(token.tokenType == EQUALtk){
        
        node->tokens = token;
        token = scan(fp);
        return node;
    }
    else if(token.tokenType == DIFFtk){
        
        node->tokens = token;
        token = scan(fp);
        return node;
    }
    else{
        
        printf("Error expected LESStk, LESSEQtk, GREATERtk, GREATEREQtk, EQUALtk, or DIFFtk \n");
        printf("instead of %s on line #%d\n", token.intst, token.lineNum);
        exit(1);
    }
}

Node *assign(){
    
    Node *node = getNode(assignNode);
    
    if(token.tokenType = IDtk){
        
        int exists = verify(token.intst);
        if(exists == -1){
            printf("Variable '%s' on line#: %d was not declared in the current scope\n", token.intst, token.lineNum);
            exit(1);
        }
        node->tokens = token;
        token = scan(fp);
        
        if(token.tokenType == ASSIGNtk){
            
            token = scan(fp);
            node->child1 = expr();
            if(token.tokenType == SEMICOLONtk){
                
                
                token = scan(fp);
                return node;
            }
            else{
                printf("Error expected SEMICOLONtk instead of %s on line #%d\n", token.intst, token.lineNum);
                exit(1);
            }
        }
        else{
            printf("Error expected ASSIGNtk instead of %s on line #%d\n", token.intst, token.lineNum);
            exit(1);
        }
    }
    else{
        printf("Error expected IDtk instead of %s on line #%d\n", token.intst, token.lineNum);
        exit(1);
    }
    
}

Node *loop(){
    
    Node *node = getNode(loopNode);
    
    if(token.tokenType == WHILEtk){
        
        token = scan(fp);
        
        if(token.tokenType == LEFTPAtk){
            
            token = scan(fp);
            node->child1 = expr();
            node->child2 = RO();
            node->child3 = expr();
            
            if(token.tokenType == RIGHTPAtk){
                
                token = scan(fp);
                node->child4 = block();
                return node;
            }
            else{
                printf("Error expected RIGHTPAtk instead of %s on line #%d\n", token.intst, token.lineNum);
                exit(1);
            }
        }
        else{
            printf("Error expected LEFTPAtk instead of %s on line #%d\n", token.intst, token.lineNum);
            exit(1);
        }
    }
    else{
        printf("Error expected WHILEtk instead of %s on line #%d\n", token.intst, token.lineNum);
        exit(1);
    }
}

//Gets the node type and adds to the parsetree
Node *getNode(nodeType type){
    
    Node *node;
    node = (Node *) malloc(sizeof(Node));
    
    node->nodeType = type;
    
    
    node->child1 = NULL;
    node->child2 = NULL;
    node->child3 = NULL;
    node->child4 = NULL;
    
    return node;
    
}

//gets token info and returns the token pointer
Token *getToken(Token token){
    
    Token *tkPTR = (Token *) malloc(sizeof(Token));
    strcpy(tkPTR->intst, token.intst);
    tkPTR->lineNum = token.lineNum;
    tkPTR->tokenType = token.tokenType;
    return tkPTR;
}

//Adds a token to the current linked list
//tokens that should be added to the linked list should only be variables(IDtk)
//and numbers(NUMtk).



