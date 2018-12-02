#include "Source.h"


int myshell_cd(char* line, char* args)
{
	if (args == NULL)
	{
		fprintf(stderr, "my_shell: expected argument to \"cd\"\n");
	}
	else
		if (!SetCurrentDirectory(args))
			fprintf(stderr, "my shell: bad input to \"cd\"; Error: %d\n", GetLastError());

	return 1;
}

int myshell_exit(char* line, char* args)
{
	return 0;
}

int myshell_help(char* line, char* args)
{
	printf("Welcome to my_shell\nTo call program, simple print it name and arguments;\nBuiltin functionality:\n");
	for (int i = 0; i < builtin_num(); ++i)
		printf(" - %s\n", builtInNames[i]);

	return 1;
}