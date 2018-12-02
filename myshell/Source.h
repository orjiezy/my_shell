#pragma once
#ifndef SOURCE_H
#define SOURCE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "windows.h"

#define LINE_BUFFER_SIZE 256


// BuiltINS
int myshell_cd(char* line, char* args);

int myshell_exit(char* line, char* args);

int myshell_help(char* line, char* args);

// Starts the loop that takes an input from user every iteration, parse it and tries to do anything user asks this
int shell_loop();

// Reading the line from user's input
char* read_line();

// Decides, if the command is builtIn, execute builtIn, else create the process of ...
int execute_command(char* line);

int create_process(char* line);

// @brief 
// @param args is whatever is going after callee name
// @return first thing before whitespace;
char* getCalleeName(char* line, char** args);

inline int builtin_num();

// array of names of the builtin functions;
const static char *builtInNames[] =
{
	"cd",
	"exit",
	"help"
};


#endif