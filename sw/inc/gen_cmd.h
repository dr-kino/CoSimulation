#ifndef GEN_CMD_H_
#define GEN_CMD_H_

#include<stdint.h>
#include<stddef.h>

/* Command dictionary entry */
struct commands_t
{
	/* Expected command line */
	const char* cmd;
	/* Command key, each command must have a distintic one */
	const int key; 
	/* Command help shown when help is invoked */
	const char* help;
	/* Extended POSIX regex to match command line in cmd field */
	const char* pattern;
};

#define null_key -1
#define first_key 0

/* Public functions prototypes */
void show_commands(const struct commands_t*);
int command_parse(const struct commands_t*, const char*, char**, size_t);
char* command_prompt(void);

#endif /* GEN_CMD_H_ */

