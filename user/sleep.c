//
// Created by cl on 2020/9/19.
/*
 * Before you start coding, read Chapter 1 of the xv6 book.
 * Look at some of the other programs in user/ (e.g., user/echo.c, user/grep.c, and user/rm.c) to see how you can obtain the command-line arguments passed to a program.
 * If the user forgets to pass an argument, sleep should print an error message.
 * The command-line argument is passed as a string; you can convert it to an integer using atoi (see user/ulib.c).
 * Use the system call sleep.
 * See kernel/sysproc.c for the xv6 kernel code that implements the sleep system call (look for sys_sleep), user/user.h for the C definition of sleep callable from a user program, and user/usys.S for the assembler code that jumps from user code into the kernel for sleep.
 * Make sure main calls exit() in order to exit your program.
 * Add your sleep program to UPROGS in Makefile; once you've done that, make qemu will compile your program and you'll be able to run it from the xv6 shell.
 * Look at Kernighan and Ritchie's book The C programming language (second edition) (K&R) to learn about C.
*/
#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

void
my_sleep(int time){
    if (time <= 0){
        fprintf(2,"A proper time should be given\n");
        exit(1);
    }
    sleep(time);
    exit(0);
}

int
main(int argc, char* argv[]){
    if (argc <= 1){
        my_sleep(-1);
        exit(0);
    }
    int sleep_time = atoi(argv[1]);
    my_sleep(sleep_time);
    exit(0);
}