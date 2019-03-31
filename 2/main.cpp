#include "parser.h"
#include <sys/wait.h>
#include <fcntl.h>

#define LEN 1024
#define READ_END 0
#define WRITE_END 1

int exe_cmd(struct cmd *, int[2], int[2], int = 0);
int exe_cd(char **);
int exe_exit(char **);
int exe_help(char **);

const char * other_cmd[] = {
	"cd"
};

int (*exe_other_cmd[])(char **) = {
	&exe_cd
};

int other_cmd_size = sizeof(other_cmd) / sizeof(const char *);
int EXIT = 0;

int main(int argc, char ** argv) {
	struct cmd * head = 0;
	int res = 0;

/*
	FILE * fp = fopen("b.txt", "w");
	if (!fp) {
		fprintf(stderr, "open error\n");
		return 0;
	}
*/
	
	while (res >= 0 && EXIT == 0) {
		head = parse();
		if (!head) break;
/**
		if (!head) {
			printf("memory error\n");
			return 2;
		}
		print_cmd(head);
*/

		int fd1[2];
		int fd2[2];
		pipe(fd1);

		res = exe_cmd(head, fd1, fd2);
//fprintf(stderr, "%d %d\n", res, EXIT);

		delete_cmd(head);
	}

	return EXIT;
}

int exe_cmd(struct cmd * curr, int fd1[2], int fd2[2], int op) {
	pipe(fd2);
	int child = fork();

	if (child < 0) {
//		fprintf(stderr, "fork error for %s cmd\n", curr->argv[0]);
		return 1;
	} else if (child == 0) {
		close(fd1[WRITE_END]);
		close(fd2[READ_END]);

		switch (op) {
		case PIPE: dup2(fd1[READ_END], READ_END);
			break;
		case REDIR1:
		case REDIR2:
			close(fd1[READ_END]);
			close(fd2[WRITE_END]);
			return 0;
		default: close(fd1[READ_END]);
		}

		int flags = O_RDWR | O_CREAT | O_NONBLOCK;
		switch (curr->op) {
		case PIPE:
			dup2(fd2[WRITE_END], WRITE_END);
			break;
		case REDIR2:
			flags |= O_APPEND;
		case REDIR1:
			close(fd2[WRITE_END]);
			fd2[WRITE_END] = open(curr->next->argv[0], flags, 0777);
			if (fd2[WRITE_END] < 0) {
//				fprintf(stderr, "%s cmd error\n", curr->argv[0]);
				return 1;
			}
			dup2(fd2[WRITE_END], WRITE_END);
		default: close(fd2[WRITE_END]);
		}

		for (int i = 0; i < other_cmd_size; i++)
			if (!strcmp(curr->argv[0], other_cmd[i]))
				return (*exe_other_cmd[i])(curr->argv);
		execvp(curr->argv[0], curr->argv);
//		fprintf(stderr, "%s cmd error\n", curr->argv[0]);
		return 1;
	}

	int status = 0;
	close(fd1[READ_END]);
	close(fd1[WRITE_END]);
	waitpid(child, &status, 0);
	switch (curr->op) {
		case AND:
			if (status == 0) return exe_cmd(curr->next, fd2, fd1, AND);
			break;
		case OR:
			return exe_cmd(curr->next, fd2, fd1, OR);
			break;
		case PIPE:
			return exe_cmd(curr->next, fd2, fd1, PIPE);
			break;
		case REDIR1:
			return exe_cmd(curr->next, fd2, fd1, REDIR1);
			break;
		case REDIR2:
			return exe_cmd(curr->next, fd2, fd1, REDIR2);
			break;
	}
	close(fd2[READ_END]);
	close(fd2[WRITE_END]);
	return status;
}

int exe_cd(char ** argv) {
	return chdir(argv[1]);
}

int exe_exit(char ** argv) {
	EXIT = -1;
	return -1;
}

