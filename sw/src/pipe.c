#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<argp.h>
#include<argz.h>

/*******************************************************************************
***  Global Variables  *********************************************************
*******************************************************************************/
const char *argp_program_version =
"CoSimulation 0.1";
const char *argp_program_bug_address =
"<doctorkinoo@gmail.com>";
     
/* Program documentation. */
static char doc[] =
"Command line application to test 8 Bit Adder hardware.";

enum key_options
{
	key_verbose = 'v',
	key_avalue = 'a',
	key_bvalue = 'b',
	key_sum = 's',
	/* Force to stay away from ordinary printable ascii codes */
	key_pipein = 128,
	key_pipeout
};

/* The options we understand. */
static struct argp_option options[] = 
{
	{"verbose", key_verbose, 0, 0,  "Produce verbose output", 0},
	{"avalue",  key_avalue, "A_VALUE", 0, "Value A to sum", 0},
	{"bvalue",  key_bvalue, "B_VALUE", 0, "Value B to sum", 0},
	{"sum", key_sum, "SUM", 0, "Execute SUM command", 0},
	{"pipein", key_pipein, "PIPEIN", 0, "Pipe from simulated hardware", 0},
	{"pipeout", key_pipeout, "PIPEOUT", 0, "Pipe to simulated hardware", 0},
	{ 0, 0, 0, 0, 0, 0}
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
	       printf("Use alphadata card: %s\n", arguments->avalue ? "yes" : "no"); 
	       printf("Alphadata card number: \"%s\"\n", arguments->bvalue ? arguments->bvalue : ""); 
	       printf("Execute command: %s\n", arguments->sum ? "yes" : "no");
	       printf("Input pipe: \"%s\"\n", arguments->pipein_arg ? arguments->pipein_arg : "");
	       printf("Output pipe: \"%s\"\n", arguments->pipeout_arg ? arguments->pipeout_arg : "");
	}
}

/* execute fpga2 command */
static int exec_command(struct arguments* arguments)
{
	int result = 0;
	int cmdresult = 0;

	/* Execute command if any available */
	if (arguments->sum)
	{
		/* Execute command */
	}

	/* return result */
	return result;
}

int main(int argc, char **argv)
{
	struct arguments arguments;
	int result = 0;

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

