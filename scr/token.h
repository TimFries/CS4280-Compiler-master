/*
 * Name:  Tim Fries
 * Assignment:  P1-Compil
 * 
 * File:  token.h
 * Created on Mar 9, 2017, 12:41:03 AM
 * 
 */


#ifndef TOKEN_H
#define TOKEN_H

extern int lineNum;
extern int numToken;

//Enum used for the types of tokens found by the scanner and FSA table
//Token types are set to 1000+ so the scanner understands that the next state
//about to be scanned in (by the scanner) is a final state.
typedef enum{

	IDtk=1000,

	STARTtk=1001, STOPtk=1002, DECISIONtk=1003, WHILEtk=1004, VOIDtk=1005, INTtk=1006,
	RETURNtk=1007, READtk=1008, PRINTtk=1009, PROGRAMtk=1010,

	NUMtk=1011,

	EQUALtk=1012, GREATEQtk=1013, LESSEQtk=1014, DIFFtk=1015, LESStk=1016, GREATERtk=1017,

	DOTtk=1018, COMMAtk=1019, LEFTBRACEtk=1020, RIGHTBRACEtk=1021, SEMICOLONtk=1022, LEFTBRACKETtk=1023,
	RIGHTBRACKETtk=1024, RIGHTPAtk=1025, LEFTPAtk=1026,

	COLONtk=1027, ADDtk=1028, MULTtk=1029, DIVIDEtk=1030, REMAINERtk=1031, ASSIGNtk=1032, SUBtk=1033,

	EOFtk=1034,
	UNKNOWNtk=1035,
	ANDtk=1036
	
} TkType;

typedef struct tokenInfo{

	char intst[8];
	int lineNum;
	TkType tokenType;
	

} Token;


extern Token *tokens;




#endif /* TOKEN_H */

