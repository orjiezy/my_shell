#include "Source.h"

// TODO: output the current directory at every line like in a real console
// ls

// array of the builtin functions;
int(*builtInFunctions[]) (char* line, char* args) = 
{
	&myshell_cd,
	&myshell_exit,
	&myshell_help
};

//returns number of builtin functions
inline int builtin_num()
{
	return sizeof(builtInNames) / sizeof(char*);
}


int shell_loop()
{
	int status{ 1 };
	char* line;

	do 
	{
		line = read_line();
		status = execute_command(line);
		free(line);
	} while (status); // to end the program, status should be 0, i.e. function that return 0 ends the loop of the shell
	
	return 0;
}

char* read_line()
{
	// read char by char, then, if char number is more than allocated memory, realloc;

	int bufferSize = LINE_BUFFER_SIZE;
	char* line = (char *)malloc(sizeof(char) * bufferSize);
	int c; // char is int because EOF is an integer.
	int iter{ 0 };

	if (!line)
	{
		fprintf(stderr, "myshell: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while((c = getchar()) != EOF && c != '\n')
	{
		*(line + iter) = c; // try if "*(line + iter++)"  works
		++iter;

		if (iter > bufferSize)
		{
			bufferSize += LINE_BUFFER_SIZE;
			line = (char *)realloc(line, bufferSize);

			if (!line)
			{
				fprintf(stderr, "myshell: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	*(line + iter) = '\0';
	
	return line;
}

int execute_command(char* line)
{
	if (line == NULL)
	{
		return 1;
	}
	else
	{
		// parsing the line for callee name;
		char* args;
		char* name = getCalleeName(line, &args);

		for (int i = 0; i < builtin_num(); ++i)
			if (!strcmp(name, builtInNames[i]))
				return (*builtInFunctions[i])(name, args);
	}
	// Creating the process
	return create_process(line);
}

int create_process(char* line)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	RtlZeroMemory(&si, sizeof(si));
	RtlZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESIZE | STARTF_USEPOSITION;
	si.dwX = 0;
	si.dwY = 0;
	si.dwXSize = 100;
	si.dwYSize = 100;
	si.lpTitle = (LPSTR)"Caption";


	// Start the child process
	if (!CreateProcessA(NULL, // No module name (use command line)
		line, // Command line
		NULL, // Process handle not inheritable
		NULL, // Thread handle not inheritable
		FALSE, // Set handle inheritance to FALSE
		0, // No creation flags
		NULL, // Use parent’s environment block
		NULL, // Use parent’s starting directory
		&si, // Pointer to STARTUPINFO structure
		&pi) // Pointer to PROCESS_INFORMATION structure
		)
	{
		/*DWORD* error;
		GetExitCodeProcess(pi.hProcess, &error);*/
		fprintf(stderr, "myshell: Can't create the process.\n%d\n", GetLastError());
	}
	else
		WaitForSingleObject(pi.hProcess, INFINITE);

	return 1;
}

char* getCalleeName(char* line, char** args)
{
	int i{ 0 };
	int bufferSize = LINE_BUFFER_SIZE;
	char* tempArgs = (char*)malloc(sizeof(char) * bufferSize);
	if (!tempArgs)
	{
		fprintf(stderr, "myshell: allocation error\n");
		exit(EXIT_FAILURE);
	}

	char* name = (char*)malloc(sizeof(char) * bufferSize);
	if (!name)
	{
		fprintf(stderr, "myshell: allocation error\n");
		exit(EXIT_FAILURE);
	}

	// getting the name of the function
	{
		int bufferSize = LINE_BUFFER_SIZE;
		char c;
		while (((c = line[i]) != ' ') && (c != '\0'))
		{
			name[i] = c;

			++i;
			if (i > bufferSize)
			{
				bufferSize += LINE_BUFFER_SIZE;
				name = (char*)realloc(name, sizeof(char) * bufferSize);
			}
		}
		name[i] = '\0';
	}

	++i;

	// getting the args of the function
	{
		int bufferSize = LINE_BUFFER_SIZE;
		int k{ 0 };
		char c;
		if (((c = line[i]) == '\0'))
			tempArgs = NULL;
		else
		{
			while (((c = line[i]) != '\0'))
			{
				tempArgs[k] = c;

				++k;
				++i;
				if (k > bufferSize)
				{
					bufferSize += LINE_BUFFER_SIZE;
					tempArgs = (char*)realloc(tempArgs, sizeof(char) * bufferSize);
				}
			}
			tempArgs[k] = '\0';
		}
		
	}
	*args = tempArgs;
	return name;
}

