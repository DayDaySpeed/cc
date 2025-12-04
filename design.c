//small compiler for c in win64
#include <fcntl.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int token;
char *src,*old_src;
int poolsize;
int line;




void next(){

    token = *src++;//先对src取值，然后src指针右移

    return;
}

void expression(int level){
    //TODO
}

void program(){
    next();
    while(token > 0){
        printf("token is: %c\n",token);
        next();
    }
}


//virtual machine
int eval(){
    return 0;
}


int main(int argc,char **argv){

    int i, fd;

    //改成默认指向第二个命令行参数
    --argc;
    ++argv;

    poolsize = 256 * 1024;//alloc 256kb as buf
    line = 1;

    if((fd = open(*argv,O_RDONLY)) < 0){
        printf("could not open(%s)\n",*argv);
        return -1;
    }

    if(!(src = old_src = malloc(poolsize))){
        printf("could not mallc (%d) for source area\n",poolsize);
    }

    if((i = read(fd,src,poolsize-1)) <= 0){
        printf("read faliure returned %d",i);
        return -1;
    }

    src[i] = 0;
    close(fd);



    program();

    return eval();
}