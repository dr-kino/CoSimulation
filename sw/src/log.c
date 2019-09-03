#include<stdlib.h>
#include<stdio.h>
#include"log.h"

struct _LOG_Obj_t
{

};

inline void LOG_Warning(pLOG_Obj_t pMy, const char* pName, unsigned int line, const char* pWarning)
{
	printf("%s line %d warning: %s\n", pName, line, pWarning);
}

inline void LOG_PtrCheck(pLOG_Obj_t pMy, void* pVoid, const char* pName, unsigned int line)
{
	if (pVoid == NULL)
	{
		LOG_Warning(pMy, pName, line, "NULL pointer.");
	}
}

