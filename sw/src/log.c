#include<stdlib.h>
#include<stdio.h>
#include"log.h"

struct _LOG_Obj_t
{

};

void LOG_Warning(pLOG_Obj_t pMy, const char* pName, unsigned int line, const char* pWarning)
{
	printf("%s line %d warning: %s\n", pName, line, pWarning);
}

void LOG_PtrCheck(pLOG_Obj_t pMy, void* pVoid, const char* pName, unsigned int line)
{
	if (pVoid == NULL)
	{
		LOG_Warning(pMy, pName, line, "NULL pointer.");
	}
}

