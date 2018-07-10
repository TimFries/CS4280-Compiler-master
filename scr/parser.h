#ifndef PARSER_H
#define PARSER_H

#include "token.h"

//Enum used for the types of node associated with the BNF
typedef enum {

	proNode, blockNode, varNode, mvarNode, expNode, mNode, tNode, fNode,
	rNode, statsNode, mstatNode, statNode, inNode, outNode, ifNode, loopNode,
	assignNode, roNode

} nodeType;

//main struct used to generate the parseTree that will be printed
typedef struct nodeInfo {

	nodeType nodeType;
	Token tokens;
	struct nodeInfo *child1;
	struct nodeInfo *child2;
	struct nodeInfo *child3;
	struct nodeInfo *child4;
}Node;



//All prototypes needed by the parser and BNF
Node *parser(FILE *);
Node *getNode(nodeType);
Token *getToken(Token);
void addToken(Token, Node*);
Node *program();
Node *block();
Node *vars();
Node *mvars();
Node *expr();
Node *M();
Node *T();
Node *F();
Node *R();
Node *stats();
Node *mStat();
Node *stat();
Node *in();
Node *out();
Node *ifTK();
Node *loop();
Node *assign();
Node *RO();


#endif

