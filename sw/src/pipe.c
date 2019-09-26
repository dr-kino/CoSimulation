/**
 * @file pipe.c
 * @author Rafael Cavalcanti <doctorkinoo@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 * 
 * The main file called pipe.c that implements interface with user.
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <argz.h>
#include <readline/readline.h>
#include <regex.h>
#include "gen_cmd.h"
#include "pipeBus.h"
#include "sanity.h"
#include "log.h"

/*******************************************************************************
***  Defintions  ***************************************************************
*******************************************************************************/
#define A_Address 1
#define B_Address 2

/*******************************************************************************
***  Global Variables  *********************************************************
*******************************************************************************/
const char *argp_program_version =
"CoSimulation 1.0";
const char *argp_program_bug_address =
"<doctorkinoo@gmail.com>";
static const char gModName[] =
"Test_pipeBus";

/* Pipe object */
pPIPE_Obj_t pPipe;

/* Program documentation. */
static char doc[] =
"Command line application to test 8 Bit Adder hardware.";

enum key_options
{
	key_verbose = 'v',
	key_avalue = 'a',
	key_bvalue = 'b',
	key_sum = 's',
	key_pipein = 'i',
	key_pipeout = 'o',
	key_help = 'h',
	key_quit = 'q',
	/* Force to stay away from ordinary printable ascii codes */
	key_last = 128
};

/* The options we understand. */
static struct commands_t options[] =
{
	{"verbose", key_verbose, "Produce verbose output", "^\\s*(verbose)\\s*$"},
	{"avalue A",  key_avalue, "Value A (A<=255) to sum operation", "^\\s*(avalue)\\s+([0-9]*$)+\\s*$"},
	{"bvalue B",  key_bvalue, "Value B (B<=255) to sum operation", "^\\s*(bvalue)\\s+([0-9]*$)+\\s*$"},
	{"sum", key_sum, "Execute \"sum\" command to get result for sum operation between A and B", "^\\s*(sum)\\s*$"},
	{"pipein", key_pipein, "Pipe from simulated hardware", "^\\s*(pipein)\\s*$"},
	{"pipeout", key_pipeout, "Pipe to simulated hardware", "^\\s*(pipeout)\\s*$"},
	{"help", key_help, "Display help", "^\\s*(help)\\s*$"},
	{"quit", key_quit,"Exit prompt command", "^\\s*(quit)\\s*$"},
	{ 0, 0, 0, 0}
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
	/* Set verbose level */
	int verbose;
	/* Value A */
	char* avalue;
	/* Value B */
	char* bvalue ;
	/* Enable sum */
	bool sum;
	/* Pipe to send data to simulated hardware */
	char* pipeout_arg;
	/* Pipe to receive data from simulated hardware */
	char* pipein_arg;
};

/* This prototype is here since it is required by argp */
static error_t parse_opt(int, char*, struct argp_state*);

/* argp parser. */
static struct argp argp = { options, parse_opt, 0, doc, 0, 0, 0 };

/* Parse command line options */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	know is a pointer to our arguments structure. */
	struct arguments *arguments = state->input;

	switch (key)
	{
	case key_verbose:
		arguments->verbose++;
		break;

	case key_avalue:
		arguments->avalue = arg;
		break;

	case key_bvalue:
		arguments->bvalue = arg;
		break;

	case key_sum:
		arguments->sum = true;
		break;

	case key_pipein:
		arguments->pipein_arg = arg;
		break;

	case key_pipeout:
		arguments->pipeout_arg = arg;
		break;

	case ARGP_KEY_ARG:
		//if (state->arg_num >= 2)
		///* Too many arguments. */
		//argp_usage (state);
		//
		//arguments->args[state->arg_num] = arg;

		break;

	case ARGP_KEY_END:
		//if (state->arg_num < 2)
		///* Not enough arguments. */
		//argp_usage (state);
		break;

	default:
		/* Unknown argument */
		return ARGP_ERR_UNKNOWN;
	}

	/* Success */
	return 0;
}

/* display selected options when verbose is enabled */
void display_options(struct arguments* arguments)
{
	if (arguments->verbose)
	{
	       printf("verbose level: %i\n", arguments->verbose);
	       printf("Type value A to sum operation: %s\n", arguments->avalue ? "yes" : "no");
	       printf("Type value B to sum operation: \"%s\"\n", arguments->bvalue ? arguments->bvalue : "");
	       printf("Execute SUM command: %s\n", arguments->sum ? "yes" : "no");
	       printf("Input pipe: \"%s\"\n", arguments->pipein_arg ? arguments->pipein_arg : "");
	       printf("Output pipe: \"%s\"\n", arguments->pipeout_arg ? arguments->pipeout_arg : "");
	}
}

/* execute command */
static int exec_command(struct arguments* arguments)
{
	int result = 0;
	int cmdresult = 0;
	int prompt = 1;
	int key = 0;
	char* pCmd = 0;
	char* subcommand[2];
	static bool verbose = false;
	uint32_t Data;

	do
	{
		/* Initialize pointers */
		for (int i = 0; i < 4; i++)
		{
			subcommand[i];
		}

		/* Execute command if any available */
		if (prompt)
		{
			pCmd = readline("Cosimulation>");
			/* Execute command */
			key = command_parse(options, pCmd, subcommand, 4);
		}

		switch(key)
		{
				case key_verbose:
					verbose = !verbose;
					printf("Toogle verbose mode: %s \n", (verbose == true) ? ("Activated") : ("Deactivated") );
					break;
				case key_avalue:
					if (strtoul(subcommand[1], 0, 10) > 255)
					{
						printf("[A value should be smaller than 256]\n");
					}
					else
					{
						if (verbose == true)
						{
							printf("A Value:%d\n", strtoul(subcommand[1], 0, 10));
						}

						/* Execute command here */
						PIPE_WriteToBus(pPipe, (uint8_t)strtoul(subcommand[1], 0, 10), A_Address);
					}
					break;
				case key_bvalue:
					if (strtoul(subcommand[1], 0, 10) > 255)
					{
						printf("[B value should be smaller than 256]\n");
					}
					else
					{
						if (verbose == true)
						{
							printf("B Value:%d\n", strtoul(subcommand[1], 0, 10));
						}
						/* Execute command here */
						PIPE_WriteToBus(pPipe, (uint8_t)strtoul(subcommand[1], 0, 10), B_Address);
					}
					break;
				case key_sum:
					if (verbose == true)
					{
						printf("Sum operation executing on FPGA ...\n");
					}
					/* Execute command here*/
					PIPE_ReadFromBus(pPipe, 0, &Data);
					printf("-> Sum Value = %d\n", Data);
					break;
				case key_pipein:
					if (verbose == true)
					{
						printf("Pipe input\n");
					}
					break;
				case key_pipeout:
					if (verbose == true)
					{
						printf("Pipe output\n");
					}
					break;
				case key_help:
					show_commands(options);
					break;
				case key_quit:
					printf("[Quit CoSimulation Program]\n");
					prompt = 0;
					break;
				default:
					printf("[Unknown or uncompleted command \"%s\", type \"help\" to display supported commands]\n", pCmd ? pCmd : "");
					break;
		}

	}while(prompt);
	/* return result */
	return result;
}

void showUsage(const char* name)
{
	printf("Usage: ");
	printf("%s <pipe in file> <pipe out file> \n", name);
}

/**
 *
 *
 *
 *
 *
 */
int main(int argc, char **argv)
{
	struct arguments arguments;
	int result = 0;

	/* Verify number of entries */
	if(argc < 2)
	{
		showUsage(gModName);
		return -1;
	}

	printf("Allocating pipe objects...\n");
	pPipe = PIPE_Alloc();

	printf("Initializing pipe objects...\n");
	if (PIPE_Init(pPipe, argv[1], argv[2]))
	{
		Warning(gModName, __LINE__, "Failure initializing simulation PIPE");
	}

	/* default options */
	memset(&arguments, 0, sizeof(arguments));

	/* parse command line */
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	/* display selected options when verbose is enabled */
	display_options(&arguments);

	/* exec command */
	if (exec_command(&arguments))
	{
		/* failure */
		result |= -1;
	}

	/* Success */
	return result;
}

