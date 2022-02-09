#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <memory>
#include <vector>
#include <fstream>
#include <time.h>
#include <cstdlib>

using namespace std;

enum tokenType {
	LEFT_PAR,RIGHT_PAR, // (,)
	PLUS,MINUS,MUL,DIV,MOD, // +,-,*,/,%
	GREA,SMAL,EQU,		// >,<,=
	AND,OR,NOT,
	ID,NUM,BOOL,		// 字母加字母or數字，純數字(含負號),#t和#f 
	PRINT,DEF,IF,FUN,
	ERROR,
	KNOWNYET
};

struct token {
	string lexeme;
	tokenType type;
	int val = 0;
	bool bVal = false;
} ;

struct variable {
	string name;
	int val = 0;
	bool bVal = false;
};

string stay = "\0";
int tokNow = 0;
int numVal = 0;
bool boolVal = false;
vector <string> inputList;
vector <token> TokList;
vector <variable> defVar;

bool exp () ;
/* 
bool FUN_IDs() {
	string stayName = "\0";
	if ( TokList[tokNow].type == LEFT_PAR ) {// 開頭是(  
		tokNow++;
		while ( TokList[tokNow].type == ID ) { // if
			for ( int i = 0 ; i < defVar.size() ; i++ ) {
				stayName = defVar[i].name;
			} // for
			
			tokNow++;
		} // while
		
		if ( TokList[tokNow].type == RIGHT_PAR ) {
			return true;
		} // if
	} // if 
	else {
		return false;
	} // else
} // FUN_IDs()

bool fun_exp () {
	if ( TokList[tokNow].type == LEFT_PAR ) { // 開頭是( 
		tokNow++;
		if ( TokList[tokNow].type == FUN ) { // fun
			tokNow++;
			if ( FUN_IDs ) { // FUN_IDs
				if ( exp() ) { // FUN-BODY
					if ( TokList[tokNow].type == RIGHT_PAR ) { // )
						
						return true;
					} // 
				} // if
			} // if
			else {
				tokNow = tokNow - 2;
			} // else
		} // if
		else {
			tokNow--;
		} // else
	} // if 
	
	return false;
} // fun_exp ()
*/ 
bool if_exp () {
	int stayNum = 0;
	bool stayBool = false;
	
	if ( TokList[tokNow].type == LEFT_PAR ) {// 開頭是(  
		tokNow++;
		if ( TokList[tokNow].type == IF ) { // if
			tokNow++;
			if ( exp() ) { // FTEST-EXP
				stayBool = boolVal;
				if ( exp() ) { // THAN-EXP
					stayNum = numVal;
					if ( exp() ) { // ELSE-EXP
						if ( stayBool ) {
							numVal = stayNum;
						} // if
						else {
							numVal = numVal;
						} // else
						
						if ( TokList[tokNow].type == RIGHT_PAR ) {
							if ( tokNow < TokList.size() - 1 ) {
								tokNow++;
							} // if
							else {
								cout << numVal << "~~~\n";
							} // if
						
							return true;
						} // if
					} // 
				} // if
			} // if
			
			return false;
		} // if
		else {
			tokNow--;
			return false;
		} // else
	} // if 
	else {
		return false;
	} // else
} // if_exp ()

bool def_stmt () {
	string nameStay = "\0";
	int stayNum = 0, hasVar =-1;
	bool stayBool = false;
	variable stayVar;
	
	if ( TokList[tokNow].type == LEFT_PAR ) { // 開頭是(
		tokNow++;
		if ( TokList[tokNow].type == DEF ) { // define
			tokNow++;
			if ( TokList[tokNow].type == ID ) { // VARIABLE
				nameStay = TokList[tokNow].lexeme;
				tokNow++;
				if ( exp() ) {
					stayNum = numVal;
					stayBool = boolVal;
					for ( int i = 0 ; i < defVar.size() ; i++ ) {
						if ( nameStay == defVar[i].name ) {
							hasVar = i;
						} // if
					} // for
					
					
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						if ( tokNow < TokList.size() - 1 ) {
							tokNow++;
						} // if
						
						if ( hasVar == -1 ) {
							stayVar.name = nameStay;
							stayVar.val = stayNum;
							stayVar.bVal = stayBool;
							defVar.push_back(stayVar);
						} // if
						else {
							defVar[hasVar].name = nameStay;
							defVar[hasVar].val = stayNum;
							defVar[hasVar].bVal = stayBool;
						} // else
						
						return true;
					} // if
				} // if
				else {
					tokNow= tokNow - 3;
				} // else
			} // if
			else {
				tokNow= tokNow - 2;
			} // else
			
			return false;
		} // if
		else {
			tokNow--;
			return false;
		} // else
	} // if 
	else {
		return false;
	} // else
} // def_stmt () 

bool log_op () {
	bool stayBool = false;
	
	if ( TokList[tokNow].type == LEFT_PAR ) { // 開頭是( 
		tokNow++;
		if ( TokList[tokNow].type == AND ) { // and
			tokNow++;
			if ( exp() ) {
				stayBool = boolVal;
				if ( exp() ) {
					stayBool = stayBool & boolVal;
					boolVal = stayBool;
					while ( exp() ) {
						stayBool = stayBool & boolVal;
						boolVal = stayBool;
					} // while
					
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						if ( tokNow < TokList.size() - 1 ) {
							boolVal = stayBool;
							tokNow++;
						} // if
						else {
							boolVal = stayBool;							
						} // else
						
						return true;
					} // if
				}  //if
			} // if
			else {
				tokNow = tokNow - 2;
			} // else
			
			return false;
		} // if
		else if ( TokList[tokNow].type == OR ) { // or
			tokNow++;
			if ( exp() ) {
				stayBool = boolVal;
				if ( exp() ) {
					stayBool = stayBool | boolVal;
					boolVal = stayBool;					
					while ( exp() ) {
						stayBool = stayBool | boolVal;
						boolVal = stayBool;
					} // while
					
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						if ( tokNow < TokList.size() - 1 ) {
							boolVal = stayBool;
							tokNow++;
						} // if
						else {
							boolVal = stayBool;							
						} // else
						
						return true;
					} // if
				}  //if
			} // if
			else {
				tokNow = tokNow - 2;
			} // else
			
			return false;
		} // else if
		else if ( TokList[tokNow].type == NOT ) { // not
			tokNow++;
			if ( exp() ) {
				stayBool = boolVal;
				boolVal = !stayBool;
				if ( TokList[tokNow].type == RIGHT_PAR ) {
					if ( tokNow < TokList.size() - 1 ) {
						boolVal = !stayBool;
						tokNow++;
					} // if
					else {
						boolVal = !stayBool;
					} // else
					return true;
				}  //if
			} // if
			else {
				tokNow = tokNow - 2;
			} // else
			
			return false;
		} // else if
		else {
			tokNow--;
			return false;
		} // else
	} // if
	else {
		return false;
	} // else
	
} // log_op () 

bool num_op () {
	int stayNum = 0;
	bool stayBool = false;
	
	if ( TokList[tokNow].type == LEFT_PAR ) { // 開頭是( 
		tokNow++;
		if ( TokList[tokNow].type == PLUS ) { // + 
			tokNow++;
			if ( exp() ) {
				stayNum = numVal;
				if ( exp() ) {
					stayNum = stayNum + numVal;
					numVal = stayNum ;
					while ( exp() ) {
						stayNum = stayNum + numVal;
						numVal = stayNum ;
					} // while
					
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						if ( tokNow < TokList.size() - 1 ) {
							numVal = stayNum ;
							tokNow++;
						} // if
						else {
							numVal = stayNum ;
						} // else
						
						return true;
					} // if
				} // if
			} // if
			else {
				tokNow = tokNow - 2;
			} // else
			
			return false;
		} // if
		else if ( TokList[tokNow].type == MINUS ) { // -
			tokNow++;
			if ( exp() ) {
				stayNum = numVal;
				if ( exp() ) {
					stayNum = stayNum - numVal;
					numVal = stayNum ;
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						if ( tokNow < TokList.size() - 1 ) {
							numVal = stayNum ;
							tokNow++;
						} // if
						else {
							numVal = stayNum ;
						} // else
						
						return true;
					} // if
				}  //if
			} // if
			else {
				tokNow = tokNow - 2;
			} // else
			
			return false;
		} // else if
		else if ( TokList[tokNow].type == MUL ) { // *
			tokNow++;
			if ( exp() ) {
				stayNum = numVal;
				if ( exp() ) {
					stayNum = stayNum * numVal;
					numVal = stayNum ;
					while ( exp() ) {
						stayNum = stayNum * numVal;
						numVal = stayNum ;
					} // while
					
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						if ( tokNow < TokList.size() - 1 ) {
							numVal = stayNum ;
							tokNow++;
						} // if
						else {
							numVal = stayNum ;
						} // else
						
						return true;
					} // if
				} // if
			} // if
			else {
				tokNow = tokNow - 2;
			} // else
			
			return false;
		} // else if
		else if ( TokList[tokNow].type == DIV ) { // /
			tokNow++;
			if ( exp() ) {
				stayNum = numVal;
				if ( exp() ) {
					stayNum = stayNum / numVal;
					numVal = stayNum ;
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						if ( tokNow < TokList.size() - 1 ) {
							numVal = stayNum ;
							tokNow++;
						} // if
						else {
							numVal = stayNum ;
						} // else
						
						return true;
					} // if
				}  //if
			} // if
			else {
				tokNow = tokNow - 2;
			} // else
			
			return false;
		} // else if
		else if ( TokList[tokNow].type == MOD ) { // mod
			tokNow++;
			if ( exp() ) {
				stayNum = numVal;
				if ( exp() ) {
					stayNum = stayNum % numVal;
					numVal = stayNum ;
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						if ( tokNow < TokList.size() - 1 ) {
							numVal = stayNum ;
							tokNow++;
						} // if
						else {
							numVal = stayNum ;
						} // else
						
						return true;
					} // if
				}  //if
			} // if
			else {
				tokNow = tokNow - 2;
			} // else
			
			return false;
		} // else if
		else if ( TokList[tokNow].type == GREA ) { // >
			tokNow++;
			if ( exp() ) {
				stayNum = numVal;
				if ( exp() ) {
					if ( stayNum > numVal ) {
						stayBool = true;
					} // if
					else {
						stayBool = false;
					} // else
					
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						if ( tokNow < TokList.size() - 1 ) {
							boolVal = stayBool;
							tokNow++;
						} // if
						else {
							boolVal = stayBool;
						} // else
						
						return true;
					} // if
				}  //if
			} // if
			else {
				tokNow = tokNow - 2;
			} // else
			
			 return false;
		} // else if ()
		else if ( TokList[tokNow].type == SMAL ) { // <
			tokNow++;
			if ( exp() ) {
				stayNum = numVal;
				if ( exp() ) {
					if ( stayNum < numVal ) {
						stayBool = true;
					} // if
					else {
						stayBool = false;
					} // else
					
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						if ( tokNow < TokList.size() - 1 ) {
							boolVal = stayBool;
							tokNow++;
						} // if
						else {
							boolVal = stayBool;
						} // else
						
						return true;
					} // if
				}  //if
			} // if
			else {
				tokNow= tokNow - 2;
			} // else
			
			return false;
		} // else if ()
		else if ( TokList[tokNow].type == EQU ) { // =
			tokNow++;
			if ( exp() ) {
				stayNum = numVal;
				if ( exp() ) {
					if ( stayNum == numVal ) {
						stayNum = numVal;
						stayBool = true;
					} // if
					else {
						stayBool = false;
					} // else
					
					while ( exp() ) {
						if ( stayNum == numVal ) {
							if ( stayBool ) {
								stayNum = numVal;
								stayBool = true;
							} // if
							else {
								stayBool = false;
							} // else
						} // if
						else {
							stayBool = false;
						} // else
							
						stayNum = stayNum + numVal;
					} // while
					
					
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						if ( tokNow < TokList.size() - 1 ) {
							boolVal = stayBool;
							tokNow++;
						} // if
						else {
							boolVal = stayBool;
						} // else
						
						return true;
					} // if
				}  //if
			} // if
			else {
				tokNow = tokNow - 2;
			} // else
			
			return false;
		} // else if ()
		else {
			tokNow--;
			return false;
		} // else
	} // if
	else {
		return false;
	} // else
	
} // num_op () 

bool exp () {
	string nameStay = "\0";
	
	if ( TokList[tokNow].type == BOOL  ) { // bool val
		boolVal = TokList[tokNow].bVal;
		tokNow++;
		return true;
	} // if
	else if ( TokList[tokNow].type == NUM ) { // numder
		numVal = TokList[tokNow].val;
		tokNow++;
		return true;
	} // else if
	else if ( TokList[tokNow].type == ID ) { // VARIABLE
		
		for ( int i = 0 ; i < defVar.size() ; i++ ) {
			nameStay = defVar[i].name;
			if ( nameStay == TokList[tokNow].lexeme ) {
				numVal = defVar[i].val;
			} // if
		} // for
		
		tokNow++;
		return true;
	} // else if
	else if ( num_op() ) { 
		return true;
	} // else if
	else if ( log_op() ) {
		return true;
	} // else if ()
	/*
	else if ( fun_exp() ) {
		return true;
	} // else if
	else if () { // FUN-CALL
		
	} // else if
	*/
	else if ( if_exp() ) { // 
		return true;
	} // else if
	else {
		return false;
	} // else
	
} // exp ()

bool print_stmt() {
	if ( TokList[tokNow].type == LEFT_PAR ) { // 開頭是(
		tokNow++;
		if ( TokList[tokNow].type == PRINT ) {
			if ( TokList[tokNow].lexeme == "print-num" ) { // print-num
				tokNow++;
				if ( exp() ) {
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						cout << numVal << "\n";
						return true;
					} // if
				} // if
			} // if
			else if ( TokList[tokNow].lexeme == "print-bool" ) { // print-bool
				tokNow++;
				if ( exp() ) {
					if ( TokList[tokNow].type == RIGHT_PAR ) {
						if ( boolVal ) {
							cout << "#t\n";
						} // if
						else {
							cout << "#f\n";
						} // else	
						return true;
					} // if
				} // if
			} // else if
			else { // 都不是 
				return false;
			} // else
		} // if
		
		return false;
	} // if
	else {
		return false;
	} // else
} // print_stmt()

bool stmt () {
	if ( exp() ) {
		return true;
	} // if 
	else if ( def_stmt() ) {
		return true;
	} // else if
	else if ( print_stmt() ) {
		return true;
	} // else if
	else {
		return false;
	} // else
} // stmt

bool program () {
	if ( stmt() ) {
		return true;
	} // if
	else {
		return false;
	} // else
} // program ()

void getToken () {
	string stay = "\0";
	string stayCh = "\0";
	string stayS = "\0";
	token stayT;
	bool isNum = true;
	bool isID = true;
	char ch = '\0';
	
	for ( int i = 0 ; i < inputList.size() ; i++ ) {
		stay = inputList[i];
		for ( int j = 0 ; j < stay.length() ; j++ ) {
			ch = stay[j];
			
			if ( ch == '(' ) {
				stayT.lexeme = ch;
				stayT.type = LEFT_PAR;
				TokList.push_back( stayT );
			} // if 
			else if ( ch == ')' ) {
				if ( stayCh != "\0" ) {
					for ( int k = 0 ; k < stayCh.length() ; k++ ) {
						stayS = stayCh[k];
						if ( stayS < "0" || stayS > "9" ) {
							if ( stayS != "-" ) {
								isNum = false;
								k = stayCh.length() ;
							} // if
						} // if
					} // for
					
					for ( int k = 0 ; k < stayCh.length() ; k++ ) {
						stayS = stayCh[k];
						if ( stayS < "a" || stayS > "z" ) {
							if ( stayS < "0" || stayS > "9" ) {
								if ( stayS != "-" ) {
									isID = false;
								} // else if
							} // if
						} // if
					} // for
							
					stayT.lexeme = stayCh;
					if ( stayCh == "#t" ) {
						stayT.bVal = true;
						stayT.type = BOOL;
					} // if
					else if ( stayCh == "#f" ) {
						stayT.bVal = false;
						stayT.type = BOOL;
					} // else if
					else if ( isNum ) {
						stayT.type = NUM;
						stayT.val = atoi( stayCh.c_str() );
					} // if
					else if ( isID ) {
						stayT.type = ID;
					} // else if
					else {
						stayT.type = KNOWNYET;
					} // else
					
					TokList.push_back( stayT );
					stayCh = "\0";
					isNum = true;
					isID = true;
				} // if	
				
				stayT.lexeme = ch;
				stayT.type = RIGHT_PAR;
				TokList.push_back( stayT );
			} // else if 
			else if ( ch == '+' ) {
				stayT.lexeme = ch;
				stayT.type = PLUS;
				TokList.push_back( stayT );
			} // else if 
			else if ( ch == '-' ) {
				if ( stayCh == "\0" ) {
					ch = stay[j + 1];
					if ( ch >= '0' && ch <= '9' ) {
						ch = stay[j];
						stayCh = stayCh + ch;
					} // if
					else {
						ch = stay[j];
						stayT.lexeme = ch;
						stayT.type = MINUS;
						TokList.push_back( stayT );
					} // else
				} // if	
				else {
					stayCh = stayCh + ch;
				} // else
			} // else if 
			else if ( ch == '*' ) {
				stayT.lexeme = ch;
				stayT.type = MUL;
				TokList.push_back( stayT );
			} // else if 
			else if ( ch == '/' ) {
				stayT.lexeme = ch;
				stayT.type = DIV;
				TokList.push_back( stayT );
			} // else if 
			else if ( ch == '>' ) {
				stayT.lexeme = ch;
				stayT.type = GREA;
				TokList.push_back( stayT );
			} // else if 
			else if ( ch == '<' ) {
				stayT.lexeme = ch;
				stayT.type = SMAL;
				TokList.push_back( stayT );
			} // else if 
			else if ( ch == '=' ) {
				stayT.lexeme = ch;
				stayT.type = EQU;
				TokList.push_back( stayT );
			} // else if
			else {
				if ( ch != ' ' && ch != '\0' && ch != '\t' && ch != '\n' && ch != '\r' ) {
					stayCh = stayCh + ch;
				} // if
				else {
					if ( stayCh == "mod" ) {
						stayT.lexeme = stayCh;
						stayT.type = MOD;
						TokList.push_back( stayT );
						stayCh = "\0";
					} // if
					else if ( stayCh == "and" ) {
						stayT.lexeme = stayCh;
						stayT.type = AND;
						TokList.push_back( stayT );
						stayCh = "\0";
					} // else if
					else if ( stayCh == "or" ) {
						stayT.lexeme = stayCh;
						stayT.type = OR;
						TokList.push_back( stayT );
						stayCh = "\0";
					} // else if
					else if ( stayCh == "not" ) {
						stayT.lexeme = stayCh;
						stayT.type = NOT;
						TokList.push_back( stayT );
						stayCh = "\0";
					} // else if
					else if ( stayCh == "#t" ) {
						stayT.lexeme = stayCh;
						stayT.bVal = true;
						stayT.type = BOOL;
						TokList.push_back( stayT );
						stayCh = "\0";
					} // else if
					else if ( stayCh == "#f" ) {
						stayT.lexeme = stayCh;
						stayT.bVal = false;
						stayT.type = BOOL;
						TokList.push_back( stayT );
						stayCh = "\0";
					} // else if
					else if ( stayCh == "if" ) {
						stayT.lexeme = stayCh;
						stayT.type = IF;
						TokList.push_back( stayT );
						stayCh = "\0";
					} // else if
					else if ( stayCh == "define" ) {
						stayT.lexeme = stayCh;
						stayT.type = DEF;
						TokList.push_back( stayT );
						stayCh = "\0";
					} // else if
					else if ( stayCh == "print-num" ) {
						stayT.lexeme = stayCh;
						stayT.type = PRINT;
						TokList.push_back( stayT );
						stayCh = "\0";
					} // else if
					else if ( stayCh == "print-bool" ) {
						stayT.lexeme = stayCh;
						stayT.type = PRINT;
						TokList.push_back( stayT );
						stayCh = "\0";
					} // else if
					else if ( stayCh == "lambda" ) {
						stayT.lexeme = stayCh;
						stayT.type = FUN;
						TokList.push_back( stayT );
						stayCh = "\0";
					} // else if
					else {
						if ( stayCh != "\0" ) {
							for ( int k = 0 ; k < stayCh.length() ; k++ ) {
								stayS = stayCh[k];
								if ( stayS < "0" || stayS > "9" ) {
									if ( stayS != "-" ) {
										isNum = false;
										k = stayCh.length() ;
									} // if
								} // if
							} // for
							
							for ( int k = 0 ; k < stayCh.length() ; k++ ) {
								stayS = stayCh[k];
								if ( stayS < "a" || stayS > "z" ) {
									if ( stayS < "0" || stayS > "9" ) {
										if ( stayS != "-" ) {
											isID = false;
										} // else if
									} // if
								} // if
							} // for
							
							stayT.lexeme = stayCh;
							if ( isNum ) {
								stayT.type = NUM;
								stayT.val = atoi( stayCh.c_str() );
							} // if
							else if ( isID ) {
								stayT.type = ID;
							} // else if
							else {
								stayT.type = KNOWNYET;
							} // else
							
							TokList.push_back( stayT );
							stayCh = "\0";
							isNum = true;
							isID = true;
						} // if
					} // else
				} // else	
			} // else
		} // for
		
		/*
		for ( int j = 0 ; j < TokList.size() ; j++ ) {
			cout << TokList[j].lexeme << "~" << TokList[j].type << "\n";
		} // for
		*/
		
		if ( program() ) {
			// cout << "好蒿爽爽!\n";
		} // if 
		else {
			cout << "syntax error\n";
		} // else
		
		TokList.clear();
		tokNow = 0;
		numVal = 0;
		boolVal = false;
	} // for
	
} // getToken ()

int main() {
	ifstream ifs;
	string fileName;
	string line = "\0";
	string stayLi = "\0";
	int parNum = 0;
	int a = 0, i = 0;
	
	cout << "請輸入指令:";
	cin >> fileName;
	fileName = fileName + ".lsp";
	ifs.open( fileName );
	
	if (!ifs.is_open()) {
        cout << "Failed to open file.\n";
    } // if 
	else {
		while ( getline( ifs, line ) ) {
			a = line.length();
			for ( i = 0 ; i < a ; i++ ) {
				if ( line[i] == '(' ) {
					parNum = parNum + 1;
				} // if
				else if ( line[i] == ')' ) {
					parNum = parNum - 1;
				} // else if
			} // for
			
			if ( parNum == 0 ) { // ()有對應 
				if ( line != " " && line != "\0" && line != "\t" && line != "\r" && line != "\n" ) {
					stayLi = stayLi + line;
					inputList.push_back( stayLi );
					stayLi = "\0";
				} // if				
			} // if
			else {
				stayLi = stayLi + line;
			} // else

		} // while
        
        ifs.close();

    } // else
	
	getToken ();
	return 0;
} // main 
