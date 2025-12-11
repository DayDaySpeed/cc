//small compiler for c in win64
#include "lexical.h"
#include "defs.h"

int token;


void program(){
    while((token = next()) != 0){
        printf("token is: %d\n", token);
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

    line = 1;

    //malloc and memset
    if(!(currentcharpointer = startcharpointer = malloc(poolsize))){
        printf("could not mallc (%d) for source area\n",poolsize);
    }

    if(!(data = malloc(poolsize))){
        printf("could not malloc (%d) for data",poolsize);
    }

    if(!(symbals = malloc(poolsize))){
        printf("could not malloc (%d) for symbals",poolsize);
    }

    memset(currentcharpointer,0,poolsize);
    memset(data, 0, poolsize);
    memset(symbals,0, poolsize);

    //read file
    if((fd = open(*argv,O_RDONLY)) < 0){
        printf("could not open(%s)\n",*argv);
        return -1;
    }
    if((i = read(fd,currentcharpointer,poolsize-1)) <= 0){
        printf("read faliure returned %d",i);
        return -1;
    }
    currentcharpointer[i] = 0;//在源码后面加个0作为结束符EOF
    close(fd);

    
    printf("hello\n");
    program();
    printf("finish\n");
    return eval();
}