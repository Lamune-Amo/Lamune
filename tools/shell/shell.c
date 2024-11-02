#include "lamune/unistd.h"
#include "lamune/sh.h"
#include "lamune/string.h"
#include "lamune/printk.h"

#define BUILTIN_SIZE 2

struct shell_args
{
	char name[16];
	void (*handler) (int argc, char *argv[]);
};

char *shell_token[16];
struct shell_args shell_builtin[BUILTIN_SIZE] = {
	{ "help", builtin_help },
	{ "hexapawn", builtin_hexapawn }
};

void shell_exec (int argc, char *argv[])
{
	printk ("%s: command not found\n", shell_token[0]);
}

/* str must not be const char */
int shell_tokenize (char *str)
{
	int index;
	
	index = 0;
	while (*str)
	{
		while (*str && (*str == ' ' || *str == '\t'))
			str++;
		shell_token[index++] = str;
		while (*str && !(*str == ' ' || *str == '\t'))
			str++;
		if (*str == '\0')
			break;
		*(str++) = '\0';
	}
	shell_token[index] = NULL;

	return index;
}

void shell (void)
{
	char buf[64];
	unsigned int size, i;
	int argc;

	while (1)
	{
		printk ("$ ");
		/* read from stdin */
		size = read (0, buf, 64);
		buf[size - 1] = 0;
		/* parse */
		argc = shell_tokenize (buf);
		/* handle */
		for (i = 0; i < BUILTIN_SIZE; i++)
		{
			if (!strcmp (shell_token[0], shell_builtin[i].name))
				break;
		}
		if (i == BUILTIN_SIZE)
			shell_exec (argc, shell_token);
		else
			shell_builtin[i].handler (argc, shell_token);
	}
}

