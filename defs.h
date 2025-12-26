#ifndef DEFS_H
#define DEFS_H
#include <fcntl.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define int64 long long

int line;
int token;
//alloc memmory size
#define poolsize 1024*1024

//file
char *startcharpointer;
char *currentcharpointer;
int currentnumber;
char *currentstringpointer;
//virtual machine
int64 *text,      // text segment
    *stack,     // stack segment
    *old_text; 
char *data;     // data segment
//virtual machine registers
int64   
*pc,            // point start
*bp,            // point bp
*sp;            // point sp
int64 ax,         //64bit general regester
    cycle;

int index_bp;   //index of bp


//lexical parse
int64 *symbals,*current_table;
//syntax parse
int etype;  //表达式的最终类型


int next(void);
int syntax(void);
int eval(void);




#endif