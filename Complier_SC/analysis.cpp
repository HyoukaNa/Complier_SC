#include "analysis.h"
#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _DIS_READER_

int sym;
extern WORD sentence[];
int ptrSen = 0;
WORD theWord;
char * sToken;
double dToken;

int next;		//pointer to array Quat
int Chain = 1;	//Chain
int Fa = 1;		//False exit
int Tr = 1;		//True exit

int length = 0;
int error_count = 0;

QUATERNION *Quat = (QUATERNION*)malloc(MAX_SENTENCE * sizeof(QUATERNION));
char *SymbolsTable[MAX_SENTENCE];
int ptr_symtable = 0;

// Tool functions
void p(char c)
{
	printf("analysis_%c: %d\n", c, sym);
}

void reader()
{
	theWord = sentence[ptrSen];
	sym = theWord.sys;
	if (sym == SYM_NUM)
	{
		dToken = theWord.val.num;
#ifndef _DIS_READER_
		printf("Reader <%d, %g>\n", sym, dToken);
#endif _DIS_READER_
	}
	else
	{
		sToken = theWord.val.token;
#ifndef _DIS_READER_
		printf("Reader <%d, %s>\n", sym, sToken);
#endif _DIS_READER_
	}
	ptrSen++;
	return;
}

void errorc()
{
	printf("error %2d: Can't define %d \n", ++error_count, sym);
	return;
}

bool Match(char *tk)
{
	reader();
	//printf("%s\n",theWord.val.token);
	switch (theWord.sys) {
	case 1:if (!strcmp(tk, "main")) return true; break;
	case 2:if (!strcmp(tk, "int")) return true; break;
	case 3:if (!strcmp(tk, "float")) return true; break;
	case 4:if (!strcmp(tk, "double")) return true; break;
	case 5:if (!strcmp(tk, "char")) return true; break;
	case 6:if (!strcmp(tk, "if")) return true; break;
	case 7:if (!strcmp(tk, "else")) return true; break;
	case 8:if (!strcmp(tk, "do")) return true; break;
	case 9:if (!strcmp(tk, "while")) return true; break;
	case 21:if (!strcmp(tk, "=")) return true; break;
	case 22:if (!strcmp(tk, "+")) return true; break;
	case 23:if (!strcmp(tk, "-")) return true; break;
	case 24:if (!strcmp(tk, "*")) return true; break;
	case 25:if (!strcmp(tk, "/")) return true; break;
	case 26:if (!strcmp(tk, "(")) return true; break;
	case 27:if (!strcmp(tk, ")")) return true; break;
	case 28:if (!strcmp(tk, "{")) return true; break;
	case 29:if (!strcmp(tk, "}")) return true; break;
	case 30:if (!strcmp(tk, ",")) return true; break;
	case 31:if (!strcmp(tk, ";")) return true; break;
	case 32:if (!strcmp(tk, ">")) return true; break;
	case 33:if (!strcmp(tk, ">=")) return true; break;
	case 34:if (!strcmp(tk, "<")) return true; break;
	case 35:if (!strcmp(tk, "<=")) return true; break;
	case 36:if (!strcmp(tk, "==")) return true; break;
	case 37:if (!strcmp(tk, "!=")) return true; break;
	case 0:if (!strcmp(tk, "#")) return true; break;
	default:break;
	}
	return false;
}

void emit(char *op, char *argv1, char *argv2, char *result)
{
	sprintf_s(Quat[next].op, "%s", op);
	sprintf_s(Quat[next].argv1, "%s", argv1);
	sprintf_s(Quat[next].argv2, "%s", argv2);
	sprintf_s(Quat[next].result, "%s", result);
	next++;
	return;
}

void printQuat()
{
	printf("-----Quad-----:\n");
	for (int i = 1; i <= next; i++) {
		printf("%d :(%s, %s, %s, %s)\n", i, Quat[i].op, Quat[i].argv1, Quat[i].argv2, Quat[i].result);
	}
}

char *newTemp()
{
	static int tempCount = 0;
	char *newName = (char *)malloc(MAX_LENGTH);
	sprintf_s(newName, MAX_LENGTH,  "Temp%d", tempCount);
	sprintf_s(SymbolsTable[ptr_symtable], MAX_LENGTH, "%s", newName);
	tempCount++;
	ptr_symtable++;
	return newName;
}

bool lookup(char *name)
{
	for (int i = 0; i < MAX_LENGTH && SymbolsTable[i] != NULL; i++)
	{
		if (strcmp(SymbolsTable[i], name))
		{
			return true;
		}
	}
	//sprintf_s(SymbolsTable[ptr_symtable], MAX_LENGTH, "%s", name);
	//ptr_symtable++;
	return false;
}

int merg(int p1, int p2)
{
	printf("merg(%d , %d)----\n", p1, p2);
	int q = p2;
	if (p2 == 0) return p1;
	else {
		//printQuat();
		while (atoi(Quat[q].result)) {
			q = atoi(Quat[q].result);
		}
		printf("link(%d , %d)----\n", q, p1);
		sprintf_s(Quat[q].result, "%d", p1);
	}
	return p2;
}

void backPatch(int p, int t)
{
	int q = p;
	int r;
	while (q != 0) {
		r = atoi(Quat[q].result);
		sprintf_s(Quat[q].result, "%d", t);
		q = r;
	}
	return;
}

// Parser functions
void parser()
{
	Quat = (QUATERNION *)malloc(256 * sizeof(QUATERNION));
	Match("main");
	Match("(");
	Match(")");
	statement_area();
	if (!Match("#"))
	{
		printf("Error: Not End the Right Way!\n");
		//return;theWord.sys != 0
	}
	printQuat();
	printf("parser success!\n");
	return;
}

//A -> {L;}
void statement_area()
{
	Match("{");
	reader();
	statement_list();
	//Match("}")
	//printf("Stmt_area: %s\n",theWord.val.token );
	if (theWord.sys != SYM_RB) printf("Error: Stmt_area\n");
		printf("Stmt_Area.Chain:  %d\n", Chain);
	return;
}

//L -> S{;S};
void statement_list()
{
	statement();
	while (theWord.sys == SYM_IF || theWord.sys == SYM_DO || theWord.sys == SYM_ID)
	{
		backPatch(Chain, next);
		statement();
		//printf("Stmt_list:%s\n",theWord.val.token );
	}
	backPatch(Chain, next);
	return;
}

void statement()
{
	//printf("stmt: %s\n",theWord.val.token );
	switch (theWord.sys) 
	{
		//if
	case SYM_IF:
		if_stmt();
		break;
		//do while
	case SYM_DO:
		do_stmt();
		break;
		//define
	case SYM_ID:
		define_stmt();
		break;
	default: break;
	}
	//printf("S.Chain:  %d\n",Chain );
	return;
}

//控制语句---------------
void condition()
{
	char op[MAX_LENGTH], argv1[MAX_LENGTH], argv2[MAX_LENGTH];
	sprintf_s(argv1, "%s", expression());
	if (theWord.sys >= SYM_BIG && theWord.sys <= SYM_NE)
	{
		sprintf_s(op, "jmp%s", theWord.val.token);
	}
	sprintf_s(argv2, "%s", expression());
	Tr = next;
	emit(op, argv1, argv2, "0");
	Fa = next;
	return;
}

void if_stmt()
{
	int tempChain, tempNext;
	Match("(");
	condition();
	emit("jmp", " ", " ", "0");
	//Match )
	//printf("If_stmt:%s\n",theWord.val.token);
	if (theWord.sys != 27) printf("Error:If_stmt\n");

	Chain = Fa;
	backPatch(Tr, next);
	tempChain = Chain;//tempChain -> C.Chain
	statement_area();//Chain -> S1.Chain
	if (Match("else"))
	{
		tempNext = next;
		emit("jmp", " ", " ", "0");
		backPatch(tempChain, next);
		tempChain = Chain;//tempChain -> S1.Chain
		tempChain = merg(tempChain, tempNext);//tempChain -> T.Chain
		statement_area();//Chain -> S2.Chain
		Chain = merg(tempChain, Chain);
		reader();
	}
	else
	{
		Chain = merg(tempChain, Chain);
		return;
	}
}

void do_stmt() 
{
	int Code;
	int tempChain;
	Code = next;
	statement_area();
	tempChain = Chain;
	printf("tempChain: %d\n", tempChain);
	Match("while");
	Match("(");
	condition();
	//Match )
	if (theWord.sys != 27) printf("Error:If_stmt\n");
	backPatch(Tr, Code);
	Chain = Fa;
	printf("Chain: %d\n", Chain);
	Match(";");
	Chain = 0;
	reader();
	return;
}

//表达式语句-------------
void define_stmt()
{
	char argv1[MAX_LENGTH], result[MAX_LENGTH];
	Chain = 0;
	sprintf_s(result, "%s", theWord.val.token);
	Match("=");
	sprintf_s(argv1, "%s", expression());
	emit("=", argv1, " ", result);
	//Match ;
	if (theWord.sys != 31) printf("Error:Define_stmt\n");
	reader();
	return;
}

//E -> TT'
char * expression()
{
	char *x = NULL;
	char op[MAX_LENGTH], argv1[MAX_LENGTH], argv2[MAX_LENGTH], result[MAX_LENGTH];
	sprintf_s(argv1, "%s", term());
	sprintf_s(result, "%s", argv1);
	// + and -
	while (theWord.sys == SYM_ADD || theWord.sys == SYM_SUB)
	{
		sprintf_s(op, "%s", theWord.val.token);
		sprintf_s(argv2, "%s", term());
		sprintf_s(result, "%s", newTemp());
		emit(op, argv1, argv2, result);
		sprintf_s(argv1, "%s", result);
	}
	x = result;
	return x;
}

char * term()
{
	char *x = NULL;
	char op[128], argv1[128], argv2[128], result[128];
	sprintf_s(argv1, "%s", factor());
	sprintf_s(result, "%s", argv1);
	// * and /
	while (theWord.sys == SYM_MUL || theWord.sys == SYM_DIV)
	{
		sprintf_s(op, "%s", theWord.val.token);
		sprintf_s(argv2, "%s", factor());
		sprintf_s(result, "%s", newTemp());
		emit(op, argv1, argv2, result);
		sprintf_s(argv1, "%s", result);
	}
	x = result;
	return x;
}

char *factor()
{
	char *x = NULL;
	char result[128];
	reader();
	if (theWord.sys == 10 || theWord.sys == 20)
	{
		if (theWord.sys == 10) {
			sprintf_s(result, "%s", theWord.val.token);
		}
		else {
			sprintf_s(result, "%10.2e", theWord.val.num);
		}
		reader();
	}
	else
	{
		//Match (
		if (theWord.sys != 26) {
			printf("Expression error!\n");
		}
		sprintf_s(result, "%s", expression());
		Match(")");
	}
	x = result;
	return x;
}


// Analysis functions

void analysis()
{
	reader();
	S();
	if (sym = SYM_END && error_count < 1)
	{
		printf("analysis success\n");
	}
	else
	{
		printf("analysis fail\n");
	}
	return;
}

void S()
{
	//main
	if (sym == SYM_MAIN)
	{
		reader();
		//
		if (sym == SYM_LS)
		{
			reader();
			if (sym == SYM_RS)
			{
				reader();
				E();
			}
			else
			{
				errorc();
			}
		}
		else
		{
			errorc();
		}
	}
	else
	{
		errorc();
	}
	return;
}

void E()
{
	if (sym == SYM_LB)
	{
		reader();
		T();
		if (sym == SYM_RB)
		{
			reader();
		}
		else
		{
			errorc();
		}
	}
	else
	{
		errorc();
	}
	return;
}

void T()
{
	bool bFirst = true;

	do {
		// stringID , if, do
		if (sym == SYM_ID || sym == SYM_IF || sym == SYM_DO)
		{
			M();
			if (sym == 31)
			{
				reader();
			}
			else
			{
				errorc();
				break;
			}
		}
		else if (bFirst)
		{
			errorc();
			break;
		}
		else
		{
			break;
		}
		bFirst = false;
	} while (1);

	//printf("T complete\n");
	return;
}

void M()
{
	// stringID
	if (sym == SYM_ID)
	{
		P();
	}
	// if
	else if (sym == SYM_IF)
	{
		I();
	}
	// do
	else if (sym == SYM_DO)
	{
		W();
	}
	return;
}

void P()
{
	// string-ID
	if (sym == SYM_ID)
	{
		reader();
		// =
		if (sym == SYM_EQU)
		{
			reader();
			A();
		}
		else
		{
			errorc();
		}
	}
	else
	{
		errorc();
	}
}

void I()
{
	// if
	if (sym == SYM_IF)
	{
		reader();
		F();
		E();
		Ix();
	}
	else
	{
		errorc();
	}
	//printf("I complete\n");
	return;
}

void Ix()
{
	// else
	if (sym == SYM_ELSE)
	{
		reader();
		E();
	}
	return;
}

void W()
{
	// do
	if (sym == SYM_DO)
	{
		reader();
		E();
		// while
		if (sym == SYM_WHILE)
		{
			reader();
			F();
		}
		else
		{
			errorc();
		}
	}
	else
	{
		errorc();
	}
	return;
}

void F()
{
	A();
	// < <= > >= == !=
	if (sym >= 32 && sym <= 37)
	{
		reader();
	}
	else
	{
		errorc();
	}
	A();
	return;
}

void A()
{
	B();
	Bx();
	return;
}

void B()
{
	C();
	Cx();
	return;
}

void Bx()
{
	//22--+, 23---
	if (sym == SYM_ADD || sym == SYM_SUB)
	{
		reader();
		B();
		Bx();
	}

	return;
}

void C()
{
	//10--str, 20--num, 26--(, 27--)
	if (sym == SYM_ID || sym == SYM_NUM)
	{
		reader();
	}
	else if (sym == SYM_LS)
	{
		reader();
		A();
		if (sym == SYM_RS)
		{
			reader();
		}
		else
		{
			errorc();
		}
	}
	else
	{
		reader();
		errorc();
	}
	return;
}

void Cx()
{
	//24--*, 25--/
	if (sym == SYM_MUL || sym == SYM_DIV)
	{
		reader();
		C();
		Cx();
	}
	return;
}

