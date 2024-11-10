#include "lamune/unistd.h"
#include "lamune/sh.h"
#include "lamune/string.h"
#include "lamune/printk.h"

#define BUILTIN_SIZE 8

struct shell_args
{
	void (*handler) (int argc, char *argv[]);
	char name[16];
	char help[32];
};

char *shell_token[16];
struct shell_args shell_builtin[BUILTIN_SIZE] = {
	{ builtin_help, "help", "" },
	{ builtin_ps, "ps", "process monitor" },
	{ builtin_mm, "mm", "memory monitor" },
	{ builtin_malloc, "malloc", "malloc test" },
	{ builtin_free, "free", "free test" },
	{ builtin_clear, "clear", "clear the screen" },
	{ builtin_hexapawn, "hexapawn", "play the heaxpawn" },
	{ builtin_sp, "sp", "test stack" }
};

void builtin_help (int argc, char *argv[])
{
	const char *str = \
		"sh, version 1.0.0-amo-lamune\n" \
		"These shell commands are defined internally. Type `help' to see this list.\n\n";
	int i;
	
	if (argc > 1)
	{
		printk ("sh: help: too many arguments\n");
		return ;
	}

	printk ("%s", str);
	for (i = 1; i < BUILTIN_SIZE; i++)
		printk ("%s: %s\n", shell_builtin[i].name, shell_builtin[i].help);
	printk ("\n");
}

void shell_exec (int argc, char *argv[])
{
	printk ("%s: command not found\n", shell_token[0]);
}

/* str must not be const char */
int shell_tokenize (char *str)
{
	int index;
	int i;
	
	index = 0;

	for (i = strlen(str) - 1; i >= 0 && (str[i] == ' ' || str[i] == '\t'); i--)
		str[i] = '\0';

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
	char *test;

	while (1)
	{
		printk ("$ ");
		/* read from stdin */
		size = read (0, buf, 64);
		buf[size - 1] = 0;
		/* parse */
		argc = shell_tokenize (buf);
		if (argc == 0)
			continue;
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

