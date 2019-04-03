#include "parser.h"

static int next_word(char *);

static int next_word(char * s) {
	static int quotes_1 = 0;
	static int quotes_2 = 0;
	static int end = 0;
	int special = 0;
	int command = 0;
	int l = 0;
	char c = 0;
	if (end) return end = 0;
	while (l < BUF_LEN - 1) {
		if ((c = getchar()) <= 0) {
			s[l] = 0;
			if (c == EOF) return -1;
			return l;
		}
		switch (c) {
			case '|':
				if (special) {
					s[l] = c;
					l++;
					special = 0;
					continue;
				}
				if (l > 0) {
					ungetc(c, stdin);
				} else {
					s[l] = c;
					l++;
					if ((c = getchar()) <= 0) {
						s[l] = 0;
						if (c == EOF) return -1;
						return l;
					}
					if (c == '|') {
						s[l] = c;
						l++;
					} else ungetc(c, stdin);
				}
				s[l] = 0;
				return l;
			case '#':
				if (special) {
					s[l] = c;
					l++;
					special = 0;
					continue;
				}
				if (l > 0) {
					s[l] = c;
					l++;
					continue;
				}
				while (c != '\n' && (c = getchar()) > 0) {
					if (c == EOF) return -1;
				}
				if (c == EOF) return -1;
				ungetc(c, stdin);
				s[l] = 0;
				return l;
			case '\n':
				if (special) {
					special = 0;
					continue;
				}
				if (quotes_1 || quotes_2) {
					s[l] = c;
					l++;
					continue;
				}
				end = 1;
				s[l] = 0;
				return l;
			case '\"':
				if (special) {
					s[l] = c;
					l++;
					special = 0;
					continue;
				}
				if (quotes_1) {
					s[l] = c;
					l++;
				} else if (quotes_2) {
					quotes_2 = 0;
					s[l] = 0;
					return l;
				} else {
					quotes_2 = 1;
				}
				break;
			case '\'':
				if (special) {
					s[l] = c;
					l++;
					special = 0;
					continue;
				}
				if (quotes_2) {
					s[l] = c;
					l++;
				} else if (quotes_1) {
					quotes_1 = 0;
					s[l] = 0;
					return l;
				} else {
					quotes_1 = 1;
				}
				break;
			case '\\':
				if (special) {
					s[l] = c;
					l++;
					special = 0;
					continue;
				}
				special = 1;
				break;
			case ' ':
				if (special) {
					s[l] = c;
					l++;
					special = 0;
					continue;
				}
				if (quotes_1 || quotes_2) {
					s[l] = c;
					l++;
				} else {
					if (l == 0) continue;
					s[l] = 0;
					return l;
				}
				break;
			default:
				special = 0;
				s[l] = c;
				l++;
		}
	}
	s[l] = 0;
	return l;
}

struct cmd * parse() {
	char buf[BUF_LEN + 1];
	std::vector<char *> argv;
	struct cmd * head = 0;
	struct cmd * curr = 0;
	struct cmd * prev = 0;
	int i = 0;
	int l = 0;
	head = new struct cmd;
	if (!head) return head;
	curr = prev = head;

	while ((l = next_word(buf)) > 0) {
		argv.push_back(new char[l + 1]);
		if (!argv.back()) {
			delete_cmd(head);
			return 0;
		}
		strcpy(argv.back(), buf);
		if (l == BUF_LEN) {
			while ((l = next_word(buf)) > 0) {
				char * tmp = argv.back();
				argv.back() = new char[l + strlen(tmp) + 1];
				if (!argv.back()) {
					delete_cmd(head);
					return 0;
				}
				strcpy(argv.back(), tmp);
				strcat(argv.back(), buf);
				delete [] tmp;
				if (l < BUF_LEN) break;
			}
		}

		if (!curr) {
			curr = new struct cmd;
			if (!curr) {
				delete_cmd(head);
				return 0;
			}
		}
		if (!strcmp(buf, "&&")) curr->op = AND;
		else if (!strcmp(buf, "||")) curr->op = OR;
		else if (!strcmp(buf, "|")) curr->op = PIPE;
		else if (!strcmp(buf, ">")) curr->op = REDIR1;
		else if (!strcmp(buf, ">>")) curr->op = REDIR2;
		if (curr->op) {
			argv.pop_back();
			argv.push_back(0);
			curr->argc = argv.size();
			curr->argv = new char * [curr->argc];
			if (!curr->argv) {
				delete_cmd(head);
				return 0;
			}
			for (i = 0; i < curr->argc; i++)
				curr->argv[i] = argv[i];
			if (curr != head) {
				prev->next = curr;
				prev = curr;
			}
			curr = 0;
			argv.clear();
		}
	}
	if (l < 0) return 0;
	if (!argv.empty()) {
		argv.push_back(0);
		curr->argc = argv.size();
		curr->argv = new char * [curr->argc];
		if (!curr->argv) {
			delete_cmd(head);
			return 0;
		}
		for (i = 0; i < curr->argc; i++)
			curr->argv[i] = argv[i];
		if (curr != head) prev->next = curr;
	}

	return head;
}

void print_cmd(struct cmd * head, FILE * fp) {
	int i = 0;
	while (head) {
		fprintf(fp, "#%d: ", head->argc);
		for (i = 0; i < head->argc; i++)
			fprintf(fp, "%s ", head->argv[i]);
		fprintf(fp, "\n");
		head = head->next;
	}
}

void delete_cmd(struct cmd * head) {
	auto curr = head;
	int i = 0;
	while (head) {
		if (head->argv) {
			for (i = 0; i < head->argc; i++) 
				if (head->argv[i]) delete [] head->argv[i];
			delete [] head->argv;
		}
		curr = head;
		head = head->next;
		delete curr;
	}
}


