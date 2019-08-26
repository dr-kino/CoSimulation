#include<stdio.h>
#include<stdlib.h>
#include"ctype.h"

/* Show program usage */
void show_usage(void)
{
	printf("Co-simulation sink\n");
	printf("\n");
	printf("Usage:\n");
        printf("  sink \"sink source\"\n");
	printf("\n");
}

static char* get_answer(FILE* pFile)
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
				printf("Warning at line %d: Unable to allocate buffer\n", __LINE__);
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

/* Print answer on screen */
static void show_answer(const char* pAnswer)
{
	printf("Answer: ");
	if (pAnswer == NULL)
	{
		printf("no answer");
	}
	else
	{
		printf("%s", pAnswer);
	}
	printf("\n");
}
	
/* Print operands */
static void show_operands(char* pAnswer)
{
	char* oper;
	unsigned int i;

	i = 0;
	//oper = PIPE_ExtractOperand(pAnswer);
	//while(oper != NULL)
	//{
	//	printf("Oper(%d): %s ", i++, oper);
	//	free(oper);
	//	oper = PIPE_ExtractOperand(pAnswer);
	//}
	printf("\n");
}

/* Delete answer if any was created */
static void free_answer(char** ppAnswer)
{
	if(*ppAnswer != NULL)
		free(*ppAnswer);

	*ppAnswer = NULL;
}

int main(int argc, const char* argv[])
{
	FILE* pipe;
	char* answer;

	/* No file open */
	pipe = NULL;
	/* No answer */
	answer = NULL;

	/* Check if right number of arguments were given */
	if (argc != 2)
	{
		show_usage();
		return -1;
	}

	/* Open pipe */
	pipe = fopen(argv[1], "r");
	if (pipe == NULL)
	{
		printf("Unable to open file %s\n", argv[1]);
		goto failure;
	}

	/* Run forever */
	while(1)
	{
		/* Get answer */
		answer = get_answer(pipe);
		/* show answer on display */
		show_answer(answer);
		/* strip answer in its operands */
		show_operands(answer);
		/* delete answer buffer */
		free_answer(&answer);
	}

failure:
	/* Close pipe */
	if (pipe != NULL)
	{
		fclose(pipe);
	}

	/* Return failure */
	return -1;
}

