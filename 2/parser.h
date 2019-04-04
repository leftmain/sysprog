#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

#define BUF_LEN 100

enum operator_ {
	NONE,
	AND,
	OR,
	PIPE,
	REDIR1,
	REDIR2
};

struct cmd {
	struct cmd * next = nullptr;
	char ** argv = 0;
	int argc = 0;
	operator_ op = NONE;
};

void delete_cmd(struct cmd *);

struct cmd * parse();
void print_cmd(struct cmd *, FILE * = stderr);

#endif

