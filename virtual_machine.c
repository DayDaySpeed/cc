#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int poolsize;
//virtual machine
int *text,      //text segment
    *stack,     //stack segment
    *old_text;
char *data;     //data segment
//virtual machine registers
uintptr_t  
*pc,            //point start
*bp,            //point the offset of stack address
*sp;            //always point the top of stack address
int ax,         //general regester
    cycle;


int main(){
    //allocate memory for virtual machine
    if(!(text = old_text = malloc(poolsize))){
        printf("could not malloc (%d) for text segment",poolsize);
    }
    if(!(data = malloc(poolsize))){
        printf("could not malloc (%d) for data segment",poolsize);
    }
    if(!(stack = malloc(poolsize))){
        printf("could not malloc (%d) for stack segment",poolsize);
    }
    //init memory
    memset(text, 0, poolsize);
    memset(data, 0, poolsize);
    memset(stack, 0, poolsize);


    //initialize virtual machine's pointers
    bp = sp = (uintptr_t *)((uintptr_t)stack + poolsize);
    ax = 0;
}
