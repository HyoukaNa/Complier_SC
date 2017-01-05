#include "scanner.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _PRINTBACK_

char buf[MAX_TEXT];
int buf_fl = 0;
char prechar;
char ch;
char aos = ch;//add or sub
char str[30] = { '\0' };
int token = 0;
char *keyword[MAX_KEYWORDS] = { "main", "int", "float", "double", "char",
"if", "else", "do", "while" };
char symbol[18] = { 0, '=', '+', '-', '*', '/', '(', ')', '{', '}', ',',';',
'>','>','<','<','=','!' };
FILE *fp;  //public file point
int scanner_error_count = 0;
WORD sentence[MAX_SENTENCE];
int pSen = 0;

void lexical()
{
	init();

	//remember the result of lexical
	fopen_s(&fp, "scanner_result.txt", "w");

	while (buf[buf_fl] != '\0')
	{
		scanner();
	}
	sentence[pSen].sys = SYM_END;
	sprintf_s(sentence[pSen].val.token, "-- END --");

	fclose(fp);

	return;
}

void scanner()
{
	token = 0;
	str[token] = '\0';
	getch();
	getbc();
	if (letter(ch))
	{
		while (letter(ch) || digit(ch))
		{
			concat();
			getch();
		}
		retract();
		int c = reserve();
		if (c != 10)
		{
			getReturn(c);
		}
		else
		{
			getReturn(10);
		}
	}
	else if (digit(ch) || ch == '+' || ch == '-')
	{
		//judge continue '+' or '-'
		if (ch == '+' || ch == '-')
		{
			aos = ch;
			getch();
			if (ch != '+' && ch != '-' && prechar != '*' && prechar != '/')
			{
				if (aos == '+')
				{
					getReturn(22);
				}
				else if (aos == '-')
				{
					getReturn(23);
				}
				else
				{
					error();
				}
				retract();
				return;
			}
			else
			{
				while (ch == '+' || ch == '-')
				{
					if (aos == '+')
					{
						getReturn(22);
					}
					else if (aos == '-')
					{
						getReturn(23);
					}
					else
					{
						error();
					}
					aos = ch;
					getch();
				}
				if (digit(ch))
				{
					retract();
					ch = aos;
					concat();
					getch();
				}
				else
				{
					retract();
					if (aos == '+')
					{
						getReturn(22);
					}
					else if (aos == '-')
					{
						getReturn(23);
					}
					else
					{
						error();
					}
					return;
				}
			}
		}

		//judge number of integer
		while (digit(ch))
		{
			concat();
			getch();
		}

		//judge number of float
		if (ch == '.')
		{
			concat();

			getch();
			if (!digit(ch))
			{
				//retract();
				//getReturn(20);
				error();
				return;
			}
			while (digit(ch))
			{
				concat();
				getch();
			}
		}

		//judge number of e
		if (ch == 'e')
		{
			concat();

			getch();
			aos = ch;
			if (ch == '+' || ch == '-')
			{
				getch();
				retract();
				if (!digit(ch))
				{
					ch = aos;
					concat();
					error();
					return;
				}
				else
				{
					ch = aos;
					concat();
					getch();
				}
			}

			while (digit(ch))
			{
				concat();
				getch();
			}
			retract();
		}
		else
		{
			retract();
		}
		getReturn(20);
	}
	else
	{
		switch (ch)
		{
		case '#':getReturn(0); break;
		case '=':
		{
			getch();
			if (ch == '=')
			{
				getReturn(36);
				break;
			}
			retract();
			getReturn(21);
			break;
		}
		case '*':getReturn(24); break;
		case '/':getReturn(25); break;
		case '(':getReturn(26); break;
		case ')':getReturn(27); break;
		case '{':getReturn(28); break;
		case '}':getReturn(29); break;
		case ',':getReturn(30); break;
		case ';':getReturn(31); break;
		case '>':
		{
			getch();
			if (ch == '=')
			{
				getReturn(33);
			}
			retract();
			getReturn(32);
			break;
		}
		case '<':
		{
			getch();
			if (ch == '=')
			{
				getReturn(35);
			}
			retract();
			getReturn(34);
			break;
		}
		case '!':
		{
			getch();
			if (ch == '=')
			{
				getReturn(37);
			}
			retract();
			break;
		}
		case '\0':
		{
			printf("Reader END \n");
			getReturn(SYM_END);
			break;
		}
			
		default:error(); break;
		}
	}


	prechar = ch;
	return;
}

void getch()
{
	ch = buf[buf_fl];
	buf_fl++;
	return;
}

void getbc()
{
	while (ch == ' ' || ch == '\n')
	{
		getch();
	}
	return;
}

void concat()
{
	str[token] = ch;
	token++;
	str[token] = '\0';
	return;
}

void concat(char var)
{
	str[token] = var;
	token++;
	str[token] = '\0';
	return;
}

bool letter(char var)
{
	if (var >= 'a' && var <= 'z' || var >= 'A' && var <= 'Z')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool digit(char var)
{
	if (var >= '0' && var <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

int reserve()
{
	for (int i = 0; i < MAX_KEYWORDS; i++)
	{
		if (strcmp(str, keyword[i]) == 0)
			return (i + 1);
	}
	return 10;
}

void retract()
{
	buf_fl--;
}

double dtb()
{
	int ptr = 0, temp, count;
	int vint = 0, vfloat = 0, ve = 0;
	double vdouble = 0.0;
	int flagint = 1, flage = 1;

	if (str[ptr] == '+')
	{
		flagint = 1;
		ptr++;
	}
	else if (str[ptr] == '-')
	{
		flagint = -1;
		ptr++;
	}

	while (digit(str[ptr]))
	{
		temp = str[ptr] - '0';
		vint = vint * 10 + temp;
		ptr++;
	}

	if (str[ptr] == '.')
	{
		ptr++;
		count = 0;
		while (digit(str[ptr]))
		{
			temp = str[ptr] - '0';
			vfloat = vfloat * 10 + temp;
			ptr++;
			count++;
		}
	}
	else if (str[ptr] != 'e')
	{
		return 1.0*vint*flagint;
	}

	if (str[ptr] == 'e')
	{
		ptr++;
		if (str[ptr] == '+')
		{
			flage = 1;
			ptr++;
		}
		else if (str[ptr] == '-')
		{
			flage = -1;
			ptr++;
		}

		while (digit(str[ptr]))
		{
			temp = str[ptr] - '0';
			ve = ve * 10 + temp;
			ptr++;
		}
	}

	//start to count
	vdouble = 1.0 * vfloat;
	for (int i = 0; i < count; i++)
	{
		vdouble = vdouble / 10;
	}
	vdouble += vint * 1.0;
	if (ve < 0)
	{
		for (int i = ve; i < 0; i++)
		{
			vdouble = vdouble / 10;
		}
	}
	else
	{
		for (int i = 0; i < ve; i++)
		{
			vdouble = vdouble * 10;
		}
	}


	return vdouble*flagint;
}

void getReturn(int num)
{
	if (num == 20)
	{
		sentence[pSen].sys = num;
		sentence[pSen].val.num = dtb();
#ifndef _PRINTBACK_
		printf("\t<%d , %g>\n", num, dtb());
#endif _PRINTBACK_
		pSen++;
	}
	else if (num == 10)
	{
		sentence[pSen].sys = num;
		sprintf_s(sentence[pSen].val.token, "%s", str);
#ifndef _PRINTBACK_
		printf("\t<%d , %s>\n", num, str);
#endif _PRINTBACK_
		pSen++;
	}
	else if (num > 20)
	{
		if (num == 33 || num == 35 || num == 36 || num == 37)
		{
			sentence[pSen].sys = num;
			sprintf_s(sentence[pSen].val.token, "%c=", symbol[num - 20]);
#ifndef _PRINTBACK_
			printf("\t<%d , %c=>\n", num, symbol[num - 20]);
#endif _PRINTBACK_
			pSen++;
		}
		else
		{
			sentence[pSen].sys = num;
			sprintf_s(sentence[pSen].val.token, "%c", symbol[num - 20]);
#ifndef _PRINTBACK_
			printf("\t<%d , %c>\n", num, symbol[num - 20]);
#endif _PRINTBACK_
			pSen++;
		}
	}
	else if (num >= SYM_MAIN && num <= SYM_WHILE)
	{
		sentence[pSen].sys = num;
		sprintf_s(sentence[pSen].val.token, "%s", keyword[num-1]);
#ifndef _PRINTBACK_
		printf("\t< %d , %s>\n", num, keyword[num-1]);
#endif _PRINTBACK_
		pSen++;
	}
	else if (num == SYM_END)
	{
		sentence[pSen].sys = num;
		sprintf_s(sentence[pSen].val.token, "END");
		pSen++;
	}
	//printf("%d", num);
	fputc((char)(num + 32), fp);
	return;
}

void init()
{
	fopen_s(&fp, "input.txt", "r");
	char fch;
	for (;;)
	{
		fch = fgetc(fp);
		if (fch == EOF)
		{
			buf[buf_fl] = '\0';
			break;
		}
		buf[buf_fl] = fch;
		buf_fl++;
	}	
	fclose(fp);
	buf_fl = 0;
	return;
}

void error()
{
	printf("error: %s \n", str);
}
