#include "../kernel/types.h"
#include "user.h"
#include "../kernel/param.h"

void xargs(int argc, char *argv[], char* stdin);

// parse on original string.
// split string into separate arguments.
// returning argument list.
// change argc as length of argument list.
char **parse(char *str, int *argc);

int main(int argc, char *argv[]){
	char buf[512];
	read(0, buf, sizeof(buf));
	if(argc < 2){
		fprintf(2, "usage: xargs [options] [command [internal arguments]]\n");
		exit(1);
	}
	xargs(argc, argv, buf);
	exit(0);
}

void xargs(int argc, char *argv[], char* stdin){
	int stdc;
	char **stdv;
	stdv = parse(stdin, &stdc);

	//// fprintf(2, "stdc: %d\nstdv:\n", stdc);
	//// for(int i = 0; i < stdc; i++)
	////	fprintf(2, "%s\t%d\n", stdv[i], i);


	char *xargv[MAXARG];	// args used for called programs
	int xargc = argc;
	// combine arguments
	for(int i = 0; i < argc - 1; ++i){
		xargv[i] = argv[i + 1];
		//// fprintf(2, "%s__%d\n", xargv[i], i);
	}

	for(int i = 0; i < stdc; ++i){
		xargv[xargc - 1] = stdv[i];
		if(fork() == 0) {
			exec(xargv[0], xargv);
		} else {
			int stat;
			wait(&stat);
		}
	}

	return;
}

char **parse(char *str, int *argc){
	int len = strlen(str);
	static char *argv[MAXARG];
	int arg_start = 0;
	*argc = 0;
	for(int i = 0; i < len; ++ i){
		if(str[i] == '\n' || str[i] == ' '){
			// find separator? change them to string ends.
			str[i] = '\0';
			if(i == arg_start){
				++ arg_start;
				continue;
			}
			argv[*argc] = malloc(i - arg_start);
			strcpy(argv[*argc], &str[arg_start]);
			arg_start = i + 1;
			++ *argc;
		}
		if(*argc > MAXARG){
			break;
		}
	}

	return argv;
}