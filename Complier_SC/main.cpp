#include "scanner.h"
#include "analysis.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	lexical();
	analysis();
	parser();
	system("pause");
	return 0;
}