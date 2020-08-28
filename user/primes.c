#include "../kernel/types.h"
#include "user.h"

// print prime number
// input is a pipe's read end.
int sub_prime(int input);

int
main(int argc, char** argv){
	int p[2];
	pipe(p);
	if(fork() == 0){
		// is child proc
		close(p[1]); // close write end
		sub_prime(p[0]);
	}
	// is parent proc
	close(p[0]); // close read end
	printf("prime 1\n");
	for(int i = 2; i < 50; ++i){
		write(p[1], &i, sizeof(int));
	}

	close(p[1]); // close write end
	int stat;
	wait(&stat);
	exit(0);
}

int
sub_prime(int input){
    int div;    // base number

    // first time read from pipe.
    // exit when failed to read
    if(read(input, &div, sizeof(int)) <= 0) exit(1);
    printf("prime %d\n", div);

    int p[2];	// pipe
    pipe(p);

    if(fork() == 0){
    	// is child proc
    	close(p[1]);	// close write end
    	sub_prime(p[0]);
    }
    // is parent proc
    // close read end
    close(p[0]);

    // read from input pipe
    for(int buf; read(input, &buf, sizeof(int)) > 0; ){
		if(buf%div == 0) {
			// is not a prime
			continue;
		}
		// cannot be divided, push it to child process
		write(p[1], &buf, sizeof(int));
    }
    // clean-ups
    close(p[1]);
	close(input);
	int stat;
    wait(&stat);
    exit(0);
}