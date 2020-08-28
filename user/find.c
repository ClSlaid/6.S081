#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"
#include "../kernel/fs.h"

// format path into name
char *fmt_name(char *path);

// find target file in path
void find(char *path, char *target);

int main(int argc, char *argv[]){
	if(argc < 2){
		fprintf(2, "usage: find <dir> <filename>\n");
		exit(1);
	}

	if(argc == 2){
		find(".", argv[1]);
		exit(0);
	}

	find(argv[1], argv[2]);
	exit(0);
}

char *fmt_name(char *path) {
	static char buf[DIRSIZ+1];
	char *p;

	// Find first character after last slash.
	for(p=path+strlen(path); p >= path && *p != '/'; --p)
		;
	++p;

	if(strlen(p)>= DIRSIZ)
		return p;
	memmove(buf, p, strlen(p));
	memset(buf+strlen(p), '\0', DIRSIZ-strlen(p));
	return buf;
}

void find(char *path, char *target){
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;

	if((fd = open(path, 0)) < 0){
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}

	if(fstat(fd, &st) < 0){
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}

	switch(st.type){
		case T_FILE:
			// is file
			if(strcmp(fmt_name(path), target) == 0){
				printf("%s\n", path);
			}
			break;
		case T_DIR:
			// is dir, iterate through it.
			if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
				printf("find: path too long\n");
				break;
			}
			strcpy(buf, path);
			p = buf+strlen(buf);
			*p++ = '/';
			while(read(fd, &de, sizeof(de)) == sizeof(de)){
				// iter in dir
				if(de.inum == 0)
					// iter returns nothing
					continue;
				if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
					// iter returns current/upper directory
					continue;
				memmove(p, de.name, DIRSIZ);
				p[DIRSIZ] = 0;
				find(buf, target);
			}
			break;
		default:
			break;
	}
	close(fd);
}
