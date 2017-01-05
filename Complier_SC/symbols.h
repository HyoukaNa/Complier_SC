#ifndef _SYMBOLS_H_
#define _SYMBOLS_H_

#define SYM_MAIN	1
#define SYM_INT		2
#define SYM_FLOAT	3
#define SYM_DOUBLE	4
#define SYM_CHAR	5
#define SYM_IF		6
#define SYM_ELSE	7
#define SYM_DO		8
#define SYM_WHILE	9
#define SYM_ID		10
#define SYM_NUM		20
#define SYM_EQU		21
#define SYM_ADD		22		/* +  */
#define SYM_SUB		23		/* -  */
#define SYM_MUL		24		/* *  */
#define SYM_DIV		25		/* /  */
#define SYM_LS		26		/* (  */
#define SYM_RS		27		/* )  */
#define SYM_LB		28		/* {  */
#define SYM_RB		29		/* }  */
#define SYM_COMMA	30		/* ,  */
#define SYM_SEM		31		/* ;  */
#define SYM_BIG		32		/* >  */
#define SYM_BE		33		/* >= */
#define SYM_SMALL	34		/* <  */
#define SYM_SE		35		/* <= */
#define SYM_EQ		36		/* == */
#define SYM_NE		37		/* != */

#define SYM_END		-1


#define MAX_LENGTH	100
#define MAX_SENTENCE 1000

typedef union WORDUNION {
	double num;
	char token[MAX_LENGTH];
}theWordUnion;

typedef struct WORD {
	int sys;
	WORDUNION val;
}theword;//----TODO *head,*shift

typedef struct QUAD
{
	char op[MAX_LENGTH];			// operation
	char argv1[MAX_LENGTH];			// first argv
	char argv2[MAX_LENGTH];			// second argv
	char result[MAX_LENGTH];		// result
}QUATERNION;


#endif _SYMBOLS_H_
