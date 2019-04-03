#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

#define DEBUG 1
#define BUF_LEN 100
#define AND		1
#define OR		2
#define PIPE	3
#define REDIR1	4
#define REDIR2	5

struct cmd {
	struct cmd * next = 0;
	char ** argv = 0;
	int argc = 0;
	int op = 0;
};

void delete_cmd(struct cmd *);

struct cmd * parse(FILE * = stdin);
void print_cmd(struct cmd *, FILE * = stderr);

#endif

