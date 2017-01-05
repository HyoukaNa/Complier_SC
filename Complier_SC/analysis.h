#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_

#include "symbols.h"

#define MAX_WORDS 1000
#define END_WORDS -1

// Tool functions
void reader();
void errorc();
bool Match();
void printQuat();
char* newtemp();
void emit(char* op, char* argv1, char* argv2, char* result);
bool lookup(char* name);
//QUATERNION* nextp;
void backPatch(int p, int t);
int merg(int p1, int p2);

// Parser functions
void statement_area();
void statement_list();
void statement();
void condition();
void if_stmt();
void do_stmt();
void define_stmt();
char * expression();
char * term();
char *factor();

// Analysis functions

void parser();
void analysis();
void S();
void E();
void T();
void Tx();
void M();
void P();
void I();
void Ix();
void W();
void F();
void A();
void B();
void Bx();
void C();
void Cx();

#endif _ANALYSIS_H_