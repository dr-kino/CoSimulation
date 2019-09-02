#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>
#include<stdbool.h>
#include<pthread.h>
#include<ctype.h>
#include"pipe.h"
#include"log.h"
#include"sanity.h"

/*******************************************************************************
 ***   Private types                                                         ***
 ******************************************************************************/

/* This module object */
struct _PIPE_Obj_t
{
	char* pInFileName;
	char* pOutFileName;
	FILE* pInFile;
	FILE* pOutFile;
	uint32_t nSync;
  	/* Bus object to access local bus */
  	BUS_Obj_t Bus;
};


/*******************************************************************************
 ***   Global variables                                                      ***
 ******************************************************************************/

/* Module name displayed when logging */
const char gModName[] = "PIPE";


/*******************************************************************************
 ***  Private functions                                                      ***
 ******************************************************************************/
static int PIPE_IsAnswerSync(const char*, uint32_t*);

static int PIPE_OpenInFile(pPIPE_Obj_t pMy, const char* pFileName)
{
	size_t size;

	if (pMy == NULL)
	{
		Warning(gModName, __LINE__, "Invalid pMy object.");
		return -1;
	}

	if (pFileName == NULL)
	{
		Warning(gModName, __LINE__, "Invalid pFileName object.");
		goto failure;
	}

	free(pMy->pInFileName);
	size = strlen(pFileName);
	pMy->pInFileName = malloc(size + 1);
	if (pMy->pInFileName == NULL)
	{
		Warning(gModName, __LINE__, "Unable to allocate memory.");
		goto failure;
	}
	strcpy(pMy->pInFileName, pFileName);

	if (pMy->pInFile != NULL)
	{
		fclose(pMy->pInFile);
		pMy->pInFile = NULL;
	}
	pMy->pInFile = fopen(pMy->pInFileName, "r+");

	if (pMy->pInFile == NULL)
	{
		Warning(gModName, __LINE__, "Unable to open file.");
		goto failure;
	}

	return 0;

failure:
	free(pMy->pInFileName);
	if (pMy->pInFile != NULL)
	{
		fclose(pMy->pInFile);
	}
	pMy->pInFileName = NULL;
	pMy->pInFile = NULL;
	return -1;
}

static int PIPE_OpenOutFile(pPIPE_Obj_t pMy, const char* pFileName)
{
	size_t size;

	if (pMy == NULL)
	{
		Warning(gModName, __LINE__, "Invalid pMy object.");
		return -1;
	}

	if (pFileName == NULL)
	{
		Warning(gModName, __LINE__, "Invalid pFileName.");
		goto failure;
	}

	free(pMy->pOutFileName);
	size = strlen(pFileName);
	pMy->pOutFileName = malloc(size + 1);
	if (pMy->pOutFileName == NULL)
	{
		Warning(gModName, __LINE__, "Unable to allocate memory.");
		goto failure;
	}
	strcpy(pMy->pOutFileName, pFileName);

	if (pMy->pOutFile != NULL)
	{
		fclose(pMy->pOutFile);
		pMy->pOutFile = NULL;
	}
	pMy->pOutFile = fopen(pMy->pOutFileName, "w");

	if (pMy->pOutFile == NULL)
	{
		Warning(gModName, __LINE__, "Unable to open file.");
		goto failure;
	}

	return 0;

failure:
	free(pMy->pOutFileName);
	if (pMy->pOutFile != NULL)
	{
		fclose(pMy->pOutFile);
	}
	pMy->pOutFileName = NULL;
	pMy->pOutFile = NULL;
	return -1;
}

static int PIPE_InitObj(pPIPE_Obj_t pMy)
{
	if (pMy == NULL)
	{
		Warning(gModName, __LINE__, "Invalid pMy object.");
		return -1;
	}

	pMy->pInFileName = NULL;
	pMy->pOutFileName = NULL;
	pMy->pInFile = NULL;
	pMy->pOutFile = NULL;

	/* First sync tag is zero */
	pMy->nSync = 0;

	/* Point all read/write functions to null */
	memset(&pMy->Bus, 0, sizeof(pMy->Bus));

	return 0;
}

pPIPE_Obj_t PIPE_Alloc(void)
{
	pPIPE_Obj_t pMy;

	pMy = malloc(sizeof(PIPE_Obj_t));

	if (pMy == NULL)
	{
		Warning(gModName, __LINE__, "Failure allocating object.");
		return NULL;
	}

	if (PIPE_InitObj(pMy))
	{
		Warning(gModName, __LINE__, "Failure initializing object.");
		free(pMy);
		return NULL;
	}

	return pMy;
}

void PIPE_Free(pPIPE_Obj_t pMy)
{
	if (pMy == NULL)
	{
		return;
	}

	free(pMy->pInFileName);
	free(pMy->pOutFileName);

	if (pMy->pInFile != NULL)
	{
		fclose(pMy->pInFile);
	}

	if (pMy->pOutFile != NULL)
	{
		fclose(pMy->pOutFile);
	}

	free(pMy);
}

int PIPE_Init(pPIPE_Obj_t pMy, const char* pInFileName, const char* pOutFileName)
{
	if (pMy == NULL)
	{
		Warning(gModName, __LINE__, "Invalid pMy object.");
		return -1;
	}

	if (PIPE_OpenInFile(pMy, pInFileName))
	{
		Warning(gModName, __LINE__, "Failure opening input file.");
		return -1;
	}

	if (PIPE_OpenOutFile(pMy, pOutFileName))
	{
		Warning(gModName, __LINE__, "Failure opening output file.");
		return -1;
	}

	/* Assign valid read from/write to bus functions */
	pMy->Bus.pWrite8 = PIPE_Write8;
	pMy->Bus.pRead8 = PIPE_Read8;
	pMy->Bus.pObj = pMy;

	/* SUccess */
	return 0;
}


int PIPE_Write(pPIPE_Obj_t pMy, uint32_t Address, uint32_t Data)
{
	int result;

	/* Is my object valid? */
	if (pMy == NULL)
	{
		/* Failure */
		return -1;
	}

	/* Is there a file to write? */
	if (pMy->pOutFile == NULL)
	{
		/* Failure */
		return -1;
	}

	/* Write command
	 *
	 * The command must be a string as follows
	 * 	W,<address in hexadecimal>,<data in hexadecimal>;
	 */
	result = fprintf(pMy->pOutFile, "W,%X,%X;", Address, Data);
	if (result < 0)
	{
		/* Failure */
		return -1;
	}

	/* Success */
	return 0;
}

static char* PIPE_GetCommand(void)
{
    int max = 20;
    char* name = (char*)malloc(max); // allocate buffer
    if (name == NULL)
    {
		Warning(gModName, __LINE__, "Failure allocating memory");
		return NULL;
    }

    printf("Command: ");

    while (true)
    { // skip leading whitespace
        int c = getchar();
        if (c == EOF)
	       break; // end of file
        if (!isspace(c))
       	{
             ungetc(c, stdin);
             break;
        }
    }

    int i = 0;
    while (true)
    {
        int c = getchar();
        if (isspace(c) || c == EOF) // at end, add terminating zero
	{
            name[i] = 0;
            break;
        }
        name[i] = c;
        if (i==max-1)
       	{ // buffer full
            max = max+max;
            name = (char*)realloc(name,max); // get a new and larger buffer
            if (name == NULL)
	    {
		   Warning(gModName, __LINE__, "Failure allocating memory");
		   return NULL;
	    }
        }
        i++;
    }

    return name;
}

static int PIPE_WriteCmd(FILE* pFile, const char* pCmd)
{
	int result;

	/* Is there any command to write? */
	if (pCmd == NULL)
	{
		/* Failure */
		Warning(gModName, __LINE__, "Invalid command.");
		return -1;
	}

	/* Is there a file to write? */
	if (pFile == NULL)
	{
		/* Failure */
		Warning(gModName, __LINE__, "No file open.");
		return -1;
	}

	/* Write command
	 *
	 * The command must be a string as follows
	 * 	W,<address in hexadecimal>,<data in hexadecimal>;
	 */
	result = fprintf(pFile, "%s\n", pCmd);
	if (result < 0)
	{
		/* Failure */
		Warning(gModName, __LINE__, "Failure writing to file.");
		return -1;
	}

	/* Flush file */
	fflush(pFile);

	/* Success */
	return 0;
}

/* Chech if answer is a synchronization acknowledge
 *
 * Arguments
 *   pAnswer - pointer to answer string
 *   pTag - pointer where to store synchronization tag
 *
 * Return
 *   a non zero value if it is a synchronization acknowledge
 *   zero if it is not a synchronization acknowledge, in this case
 *   pData return an undefined value
 */
static int PIPE_IsAnswerSync(const char* pAnswer, uint32_t* pTag)
{
	char cmd;
	size_t s;
	uint32_t tag;

	/* Do not read a null pointer */
	if (pAnswer == NULL)
	{
		Warning(gModName, __LINE__, "Pointer to invalid object.");
		return 0;
	}

	/* Read answer */
	s = sscanf(pAnswer, "%c%*[,]%x%*[;]", &cmd, &tag);

	/* Failure if unable to fill all fields */
	if (s != 2)
		return 0;

	/* Check if it is a sync answer */
	if ( (cmd != 's') && (cmd != 'S') )
		return 0;

	/* Copy tag if destination is not null */
	if (pTag != NULL)
	{
		*pTag = tag;
	}

	/* Success */
	return 1;
}


static int PIPE_IsAnswerDone(const char* pAnswer)
{
	if (pAnswer == NULL)
		return 0;

	if (strcmp(pAnswer, "D;") != 0)
		return 0;
	else
		return 1;
}

static int PIPE_IsAnswerNAck(const char* pAnswer)
{
	if (pAnswer == NULL)
		return 0;

	if (strcmp(pAnswer, "I;") != 0)
		return 0;
	else
		return 1;
}

static int PIPE_IsAnswerAck(const char* pAnswer)
{
	if (pAnswer == NULL)
		return 0;

	if (strcmp(pAnswer, "A;") != 0)
		return 0;
	else
		return 1;
}

static int PIPE_IsAnswerFetch(const char* pAnswer, uint32_t* pData)
{
	char cmd;
	size_t s;

	/* Do not read a null pointer */
	if (pAnswer == NULL)
		return 0;

	/* Read answer */
	s = sscanf(pAnswer, "%c%*[,]%x%*[;]", &cmd, pData);

	/* Failure if unable to fill all fields */
	if (s != 2)
		return 0;

	/* Check it it is a fetch answer */
	if ( (cmd != 'f') && (cmd != 'F') )
		return 0;

	/* Success */
	return 1;
}

static int PIPE_IsAnswerWait(const char* pAnswer)
{
	char cmd;
	size_t s;

	/* Do not read a null pointer */
	if (pAnswer == NULL)
		return 0;

	/* Read answer */
	s = sscanf(pAnswer, "%c%*[;]", &cmd);

	/* Failure if unable to fill all fields */
	if (s != 1)
		return 0;

	/* Check it it is a fetch answer */
	if ( (cmd != 'w') && (cmd != 'W') )
		return 0;

	/* Success */
	return 1;
}

static void PIPE_DumpAnswer(FILE* pFile)
{
	int done;
	char* ans;

	do
	{
		printf("Answer: ");

		/* Get answer */
		ans = PIPE_GetAnswer(pFile);

		/* Is done instruction answer? */
		done = PIPE_IsAnswerDone(ans);
		/* Is invalid command acknowledge? */
		done |= PIPE_IsAnswerNAck(ans);
		/* Is a sync command acknowledge? */
		done |= PIPE_IsAnswerSync(ans, NULL);

		if (ans != NULL)
			printf("%s\n", ans);
		else
			printf("No answer.");

	}while(!done);
}



/*******************************************************************************
 ****  Public functions  *******************************************************
 ******************************************************************************/

void PIPE_Interactive(pPIPE_Obj_t pMy)
{
	char* cmd;

	/* Prohibit this thread of accessing null objects */
	if (pMy == NULL)
	{
		Warning(gModName, __LINE__, "NULL argument.");
	}

	printf("Entering interactive mode\n");
	printf("\n");
	printf("Valid commands:");
	printf("  Write to bus - W,<address>,<data>;\n");
	printf("  Read from bus - R,<address>;\n");
	printf("  No Operation - N;\n");
	printf("\n");
	printf("Expected answers:\n");
	printf("  Busy - B;\n");
	printf("  Command accepted - A;\n");
	printf("  Invalid command - I;\n");
	printf("  Fetch data - F,<data>;\n");
	printf("  Command done - D;\n");
	printf("\n");

	while(true)
	{
		/* get command */
		cmd = PIPE_GetCommand();

		/* Check if should quit interative mode */
		if (cmd != NULL)
		{
			if (strcmp(cmd, "quit") == 0)
			{
				free(cmd);
				break;
			}
		}

		/* Write command */
		if (PIPE_WriteCmd(pMy->pOutFile, cmd))
		{
			Warning(gModName, __LINE__, "Unable to write command.");
		}

		/* Release buffer */
		free(cmd);

		/* Get answer */
		PIPE_DumpAnswer(pMy->pInFile);
	}
}

int PIPE_WriteToBus(pPIPE_Obj_t pMy, uint32_t Addr, uint32_t Data)
{
	char buf[64];
	char* ans;
	int done;

	/* Synchronize bus */
	PIPE_SyncPipe(pMy);

	Addr -= Addr % 4;

	snprintf(buf, sizeof(buf), "W,%X,%X;", Addr, Data);

	/* Write command */
	if (PIPE_WriteCmd(pMy->pOutFile, buf))
	{
		Warning(gModName, __LINE__, "Unable to write command.");
	}

	/* Wait command acknowledge */
	do
	{
		done = 0;
		ans = PIPE_GetAnswer(pMy->pInFile);
		if (PIPE_IsAnswerNAck(ans))
		{
			Warning(gModName, __LINE__, "Command not accepted.");
			free(ans);
			return -1;
		}
		else if (PIPE_IsAnswerAck(ans))
		{
			free(ans);
			break;
		}
		else if (PIPE_IsAnswerWait(ans))
		{
			free(ans);
		}
		else
		{
			Warning(gModName, __LINE__, "Unexpected answer.");
			Warning(gModName, __LINE__, ans);
			free(ans);
		}
	}while(true);

	/* Wait until finished command execution */
	do
	{
		/* Wait until done */
		ans = PIPE_GetAnswer(pMy->pInFile);
		done = PIPE_IsAnswerDone(ans);
		free(ans);
	}while(!done);

	/* Success */
	return 0;
}

int PIPE_ReadFromBus(pPIPE_Obj_t pMy, uint32_t Addr, uint32_t* pData)
{
	char buf[64];
	char* ans;
	int done;

	/* Synchronize bus */
	PIPE_SyncPipe(pMy);

	Addr -= Addr % 4;

	snprintf(buf, sizeof(buf), "R,%X;", Addr);

	/* Write command */
	if (PIPE_WriteCmd(pMy->pOutFile, buf))
	{
		Warning(gModName, __LINE__, "Unable to write command.");
	}

	/* Wait command acknowledge */
	do
	{
		done = 0;
		ans = PIPE_GetAnswer(pMy->pInFile);
		if (PIPE_IsAnswerNAck(ans))
		{
			Warning(gModName, __LINE__, "Command not accepted.");
			free(ans);
			return -1;
		}
		else if (PIPE_IsAnswerAck(ans))
		{
			free(ans);
			break;
		}
		else if (PIPE_IsAnswerWait(ans))
		{
			free(ans);
		}
		else
		{
			Warning(gModName, __LINE__, "Unexpected answer.");
			Warning(gModName, __LINE__, ans);
			free(ans);
		}
	}while(true);

	/* Wait until finished command execution */
	do
	{
		/* Wait until done */
		ans = PIPE_GetAnswer(pMy->pInFile);
		if (PIPE_IsAnswerFetch(ans, pData))
		{
			done = 0;
		}
		else
		{
			done = PIPE_IsAnswerDone(ans);
		}
		free(ans);
	}while(!done);

	/* Success */
	return 0;
}

int PIPE_EndSimulation(pPIPE_Obj_t pMy)
{
	char buf[64];
	char* ans;
	int done;

	snprintf(buf, sizeof(buf), "Q;");

	/* Write command */
	if (PIPE_WriteCmd(pMy->pOutFile, buf))
	{
		Warning(gModName, __LINE__, "Unable to write command.");
	}

	/* Wait command acknowledge */
	do
	{
		done = 0;
		ans = PIPE_GetAnswer(pMy->pInFile);
		if (PIPE_IsAnswerNAck(ans))
		{
			Warning(gModName, __LINE__, "Command not accepted.");
			free(ans);
			return -1;
		}
		else if (PIPE_IsAnswerAck(ans))
		{
			free(ans);
			break;
		}
		else if (PIPE_IsAnswerWait(ans))
		{
			free(ans);
		}
		else
		{
			Warning(gModName, __LINE__, "Unexpected answer.");
			Warning(gModName, __LINE__, ans);
			free(ans);
		}
	}while(true);

	/* Wait until finished command execution */
	do
	{
		/* Wait until done */
		ans = PIPE_GetAnswer(pMy->pInFile);
		done = PIPE_IsAnswerDone(ans);
		free(ans);
	}while(!done);

	/* Success */
	return 0;
}

int PIPE_Write8(void* pMy, uint32_t Addr, uint8_t Data)
{
	uint32_t a;
	uint32_t b;
	uint8_t addr;
	uint32_t maska;
	uint32_t maskb;
	
	addr = Addr % 4;
	b = Data;
	maskb = 0xFF << 8 * addr;
	maska = ~maskb;

	if (PIPE_ReadFromBus(pMy, Addr, &a))
	{
		return -1;
	}

	b <<= 8 * addr;
	a &= maska;
	a |= b & maskb;

	if (PIPE_WriteToBus(pMy, Addr, a))
	{
		return -1;
	}

	/* Success */
	return 0;
}

int PIPE_Read8(void* pMy, uint32_t Addr, uint8_t* pData)
{
	uint32_t data;
	uint8_t addr;
	
	/* Read data from bus */
	if (PIPE_ReadFromBus(pMy, Addr, &data))
	{
		/* Failure */
		return -1;
	}

	/* Convert from 32-bit to 8 bit */
	addr = Addr % 4;
	if (pData == NULL)
	{
		Warning(gModName, __LINE__, "Pointer to NULL.");
		return -1;
	}
	*pData = data >> 8 * addr;

	/*
	printf("%s line %d: 0x%X @ 0x%X\n", gModName, __LINE__, data, Addr);
	printf("%s line %d: 0x%X\n", gModName, __LINE__, addr);
	printf("%s line %d: 0x%X @ 0x%X\n", gModName, __LINE__, *pData, Addr);
	*/

	/* Success */
	return 0;
}

void* PIPE_GetRWObj(pPIPE_Obj_t pMy)
{
	return pMy;
}

/* Synchronize pipe
 *
 * Full description
 *    Synchronize pipe by:
 *    1) Send sync command plus a tag
 *    2) Wait command acknowledge
 *    3) Wait sync reply with same tag
 *    4) Wait command done
 *
 * Arguments
 *    pMy - this module handler
 */
void PIPE_SyncPipe(pPIPE_Obj_t pMy)
{
	char buf[64];
	char* ans;
	unsigned int done;
	uint32_t nsync;

	/* Avoid using null object */
	if (pMy == NULL)
	{
		Warning(gModName, __LINE__, "Unable to sync pipe.");
		return;
	}

	/* Send sync command through pipe */
	snprintf(buf, sizeof(buf), "S,%X;", pMy->nSync);

	/* Write command */
	if (PIPE_WriteCmd(pMy->pOutFile, buf))
	{
		/* Avoid hanging due to unsent command. */
		Warning(gModName, __LINE__, "Unable to write sync command.");
		return;
	}

	/* Wait until get right sync acknowledge */
	do
	{
		done = 0;
		/* Get answer */
		ans = PIPE_GetAnswer(pMy->pInFile);
		/* Is sync acknowledge? */
		if (PIPE_IsAnswerSync(ans, &nsync))
		{
			/* Does synchronization tag match? */
			if (nsync == pMy->nSync)
			{
				done = 1;
			}
		}
		free(ans);
	}while(!done);

	/* Increase synchronization tag */
	pMy->nSync++;
}

/* Get answer from co-simulation pipe
 *
 * Full description
 *
 * Arguments:
 *   pFile - object handler
 *
 * Return
 *   NULL on failure, a pointer to string on success
 *
 * Note:
 *   It is up to the caller to deallocate the returned string
 *   with free()
 */
char* PIPE_GetAnswer(FILE* pFile)
{
    int max;
    int i;
    char* answer;
    char* pnew;
    int c;
    const int rsize = 20;

    /* Initialize variables */
    answer = NULL;
    max = 0;
    i = 0;
    c = ' ';

    /* Read data from pipe until ';' is reached */
    while(c != ';')
    {
	/* Resize buffer when needed */
	if (i >= max - 1)
	{
		pnew = realloc(answer, max + rsize);
		if (pnew != NULL)
		{
			/* Buffer was resized */
			max += rsize;
			answer = pnew;
	   	}
		else
		{
			LOG_Warning(NULL, gModName, __LINE__, "Unable to allocate memory.");
		}
	}

	/* Read data from pipe */
	c = fgetc(pFile);
	if( (c != EOF) && (!isspace(c)) && (i < max - 1) )
	{
		answer[i++] = c;
	}

    }

    /* Add null character marking end of string */
    if (max > 0)
    {
	    /* Make sure i do not exceed buffer boundary */
	    if (i >= max)
	    	i = max -1;

	    /* Include end of string mark */
	    answer[i++] = 0;
    }

    /* Return answer */
    return answer;
}

/* Extract operand from buffer
 *
 * Full description
 *   Extract operand from buffer by:
 *   1) Allocate memory for operand
 *   2) Copy operand to new buffer
 *   3) Remove operand from old buffer
 *
 * Arguments
 *   pStr - pointer to string (null pointer safe)
 *
 * Return
 *   first operand from original buffer if success, or a null
 *   pointer if failure.
 *
 * Note 1: first operand and separator are removed from input string
 * Note 2: it is up to the caller to free the returned operand if
 *   is there any.
 */
char* PIPE_ExtractOperand(char* pStr)
{
	char* oper;
	size_t len;
	size_t oper_len;

	/* Nothing to do */
	if (pStr == NULL)
		return NULL;

	/* Check if there is any data on answer */
	len = strlen(pStr);
	if (len < 1)
		return NULL;

	/* Find first occurance of ',' or ';' */
	oper_len = strcspn(pStr, ",;");

	/* Allocate memory for operand */
	oper = NULL;
	if (oper_len > 0)
		oper = malloc(oper_len + 1);

	/* Copy operand to new buffer */
	if (oper != NULL)
	{
		strncpy(oper, pStr, oper_len);
		oper[oper_len] = 0;
	}

	/* Remove operand from old buffer */
	memmove(pStr, &pStr[oper_len + 1], len - (oper_len + 1));
	pStr[len - (oper_len + 1)] = 0;

	/* Return extracted operand */
	return oper;
}

/* Get read from/write to bus object
 *
 * Full description
 *   Get read from/write to bus object
 *
 * Arguments
 *   pMy - driver object
 *
 * Return
 *   Null object if invalid driver
 */
const BUS_Obj_t* PIPE_GetBusObj(pPIPE_Obj_t pMy)
{
	const BUS_Obj_t* ptr = 0;

	/* Only return bus object if there is a valid pMy object */
	if (pMy)
		ptr = &pMy->Bus;

	return ptr;
}
