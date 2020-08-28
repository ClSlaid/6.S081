//
// Created by cl on 2020/9/26.
//
#include "../kernel/types.h"
#include "user.h"

char msg[32];

int
proc(int p2c[],int c2p[]){  //process
    int child = fork();
    if(child != 0){ //is parent process
        int pid = getpid();
        strcpy(msg,"received ping");

//        close(0);dup(c2p[0]);
//        //switch read fd to child->parent pipe's read end
//        close(1);dup(p2c[1]);
//        //switch write fd to parent->child pipe's write end
        write(p2c[1],msg,strlen(msg));
        read(c2p[0],msg,strlen(msg));
        printf("%d: %s\n",pid,msg);

        exit(0);
    }else{  // is child process
        int pid = getpid();
//        close(0);dup(p2c[0]);
//        close(1);dup(c2p[1]);
        read(p2c[0],msg,20);
        printf("%d: %s\n",pid,msg);
        strcpy(msg,"received pong");
        write(c2p[1],msg,strlen(msg));
        exit(0);
    }
}

int
main(int argc, char* argv[]){
    int p2c[2]; //pipe of parent to child
    int c2p[2]; //pipe of child to parent
    pipe(p2c);
    pipe(c2p);
    proc(p2c,c2p);
    exit(0);
}