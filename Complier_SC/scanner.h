#ifndef _SCANNER_H_
#define _SCANNER_H_

#include "symbols.h"

#define MAX_KEYWORDS 9
#define MAX_STR 30
#define MAX_TEXT 1000

/*
*	functions of scanner
*/
void getch();
void getbc();
void concat();
bool letter(char var);
bool digit(char var);
int reserve();
void retract();
void getReturn(int num);
void error();
void scanner();
void lexical();
void init();
double dtb();

#endif
