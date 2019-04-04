#include "parser.h"
#include <sys/wait.h>
#include <fcntl.h>

#define LEN 1024
#define READ_END 0
#define WRITE_END 1
#define DEBUG 1

int exe_cmd(struct cmd *, int[2], int[2], int = 0);

int main(int argc, char ** argv) {
	struct cmd * head = 0;
	int res = 0;
	FILE * fp = 0;
	
	while (res >= 0) {
		head = parse();
		if (!head) break;

		int fd1[2];
		int fd2[2];
		pipe(fd1);

		res = exe_cmd(head, fd1, fd2);

		delete_cmd(head);
	}

	if (res >= 0) delete_cmd(head);
	return 0;
}

int exe_cmd(struct cmd * curr, int fd1[2], int fd2[2], int op) {
	if (curr->argc == 0) return 0;
	int status = 0;
	if (curr->argv[0] && !strcmp(curr->argv[0], "cd")) {
		chdir(curr->argv[1]);
	} else {
		pipe(fd2);
		int child = fork();

		if (child < 0) {
			return -1;
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
				return -1;
			default: close(fd1[READ_END]);
			}

			int flags = O_RDWR | O_CREAT | O_TRUNC;
			switch (curr->op) {
			case PIPE:
				dup2(fd2[WRITE_END], WRITE_END);
				break;
			case REDIR2:
				flags |= O_APPEND;
				flags &= (~O_TRUNC);
			case REDIR1:
				close(fd2[WRITE_END]);
				fd2[WRITE_END] = open(curr->next->argv[0], flags, 0777);
				if (fd2[WRITE_END] < 0) {
					return -1;
				}
				dup2(fd2[WRITE_END], WRITE_END);
			default: close(fd2[WRITE_END]);
			}

			execvp(curr->argv[0], curr->argv);
			return -1;
		}

		close(fd1[READ_END]);
		close(fd1[WRITE_END]);
		waitpid(child, &status, 0);
	}
	switch (curr->op) {
		case AND:
			if (status == 0) return exe_cmd(curr->next, fd2, fd1, AND);
			else if (curr->next->op) return exe_cmd(curr->next->next, fd2, fd1);
			break;
		case OR:
			if (status != 0) return exe_cmd(curr->next, fd2, fd1, OR);
			else if (curr->next->op) return exe_cmd(curr->next->next, fd2, fd1);
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

