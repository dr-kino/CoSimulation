#include<stdlib.h>
#include<stdio.h>
#include"sanity.h"

void Warning(const char* pName, unsigned int line, const char* pWarning)
{
	printf("%s line %d warning: %s\n", pName, line, pWarning);
}

void Error(const char* pName, unsigned int line, const char* pError)
{
	printf("%s line %d error: %s\n", pName, line, pError);
	exit(EXIT_FAILURE);
}

int SoftPtrCheck(void* pVoid, const char* pName, unsigned int line)
{
	if (pVoid == NULL)
	{
		Warning(pName, line, "NULL pointer.");
		return -1;
	}

	return 0;
}

void HardPtrCheck(void* pVoid, const char* pName, unsigned int line)
{
	if (pVoid == NULL)
	{
		Error(pName, line, "NULL pointer.");
	}
}

