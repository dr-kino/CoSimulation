#include<stdio.h>
#include<stdlib.h>
#include<argz.h>
#include<string.h>
#include<regex.h>
#include"gen_cmd.h"

/* Private functions prototypes */
static int match_command_parse(const struct commands_t*, const char*, int*, size_t, regmatch_t*);
static void show_match(const char* cmd, size_t nmatch, regmatch_t* pmatch);
static char* copy_match(const char* cmd, regmatch_t* pmatch);


/* Parse an expression to check if matches any command available in a dictionary
 * and extracted subcommand if any available.
 *
 * Full Description
 *   Parse expression to check if it matches any command available in a dicionary.
 *
 * Arguments
 *   commands - pointer to commands dictionary.
 *              It may be a null pointer, if it is not then the last pattern
 *              must point to null.
 *   cmd - null terminated string containing expression to parse, it may be
 *         a null pointer.
 *   tokens - array of null terminated strings, each one containning one token.
 *         It may be a null pointer without side effects.
 *         The last element is always set to null to make this a null termited array.
 *   ntokens - number of elements available on "tokens"
 *
 * Return
 *   key binding of matched command.
 */
int command_parse(const struct commands_t* commands, const char* cmd, char** tokens, size_t ntokens)
{
	regmatch_t* pmatch = 0;
	size_t nmatch = 0;
	int key = null_key;
	size_t i;

	/* Allocate object to hold tokens */
	if (ntokens)
	{
		pmatch = malloc((ntokens) * sizeof(regmatch_t));
		if (pmatch)
		{
			nmatch = ntokens;
		}
	}

	/* Check if expression matches any command */
	if (!match_command_parse(commands, cmd, &key, nmatch, pmatch))
	{
		/* Copy tokens if possible, it is up to the user to free
		 * the allocated strings */
		if (tokens)
		{
			/* Copy each token */
			for(i = 1; i < nmatch; i++)
			{
				tokens[i - 1] = copy_match(cmd, &pmatch[i]);
			}
			/* Last token must point to null to make this a null terminated array */
			tokens[ntokens - 1] = 0;
		}
	}
	else
	{
		/* Make all tokens null */
		if (tokens)
		{
			for(i = 0; i < ntokens; i++)
			{
				tokens[i] = 0;
			}
		}
	}

	/* Free regmatch objects */
	if (pmatch)
		free(pmatch);

	/* Return matched command key */
	return key;
}

/* Parse an expression to check if matches any command available in a dictionary
 *
 * Full Description
 *   Parse expression to check if it matches any command available in a dicionary.
 *
 * Arguments
 *   commands - pointer to commands dictionary.
 *              It may be a null pointer, if it is not then the last pattern
 *              must point to null.
 *   cmd - null terminated string containing expression to parse, it may be
 *         a null pointer.
 *   key - pointer here to store key binding of matched command, it may be a
 *         null pointer.
 *   nmatch - number of entries available on pmatch.
 *   pmatch - regex structure containg byte offset of matched command pattern
 *         groups.
 *
 * Return
 *   0 if there was a match, any other value otherwise.
 */
static int match_command_parse(const struct commands_t* commands, const char* cmd, int* key, size_t nmatch, regmatch_t* pmatch)
{
	int lkey = null_key;
	int match = 1;
	regex_t preg;

	/* Only try to parse an expression if there is a command dictionary
	 * and an expression to parse. */
	if(commands && cmd)
	{
		/* Try to match expression with any command on dictionary.
		 * A pointer to a null pattern indicate there is no more commands
		 * on commands dictionary. */
		while(commands->pattern && match)
		{
			/* Check if expression matchs current dictionary entry */
			if (!regcomp(&preg, commands->pattern, REG_EXTENDED))
			{
				match = regexec(&preg, cmd, nmatch, pmatch, 0);
			}
			regfree(&preg);
			/* If there is a match, store dictionary entry */
			if (!match)
			{
				lkey = commands->key;
				//show_match(cmd, nmatch, pmatch);
			}
			commands++;
		}
	}

	/* Store command key if possible */
	if (key)
	{
		*key = lkey;
	}

	/* Return 0 if expression matched any command, any other value otherwise */
	return match;
}

void show_commands(const struct commands_t* commands)
{
	printf("Commands:\n");
	while(commands->cmd)
	{
		printf("  %s - %s\n", commands->cmd, commands->help);
		commands++;
	} 
	printf("\n");
}

/* show match */
static void show_match(const char* cmd, size_t nmatch, regmatch_t* pmatch)
{
	size_t i;
	int j;

	/* Only try to parse an expression if there is an expression and a match */
	if(cmd && pmatch && nmatch > 0)
	{
		for(i = 0; i < nmatch; i++)
		{
			printf("match[%d]: \"", i);
			for(j = pmatch[i].rm_so; j < pmatch[i].rm_eo; j++)
			{
				printf("%c", cmd[j]);
			}
			printf("\"\n");	
		}
	}
}

/* copy matched token
 *
 * Arguments
 *   cmd - expression containing tokens
 *   pmatch - regmatch containg byte offset of token in cmd
 *
 * Return
 *   A dynamic allocated null terminated string containg token.
 *   It is up to the USER to FREE the returned string if any.
 *   This function may return null if there is no token to copy
 */
static char* copy_match(const char* cmd, regmatch_t* pmatch)
{
	char* str = 0;
	size_t len = 0;

	if (cmd && pmatch)
	{
		len = pmatch->rm_eo - pmatch->rm_so;
	}

	if (len)
	{
		str = malloc((len + 1) * sizeof(char));
	}

	if (str)
	{
		strncpy(str, &cmd[pmatch->rm_so], len);
		str[len] = 0;
	}

	return str;
}
