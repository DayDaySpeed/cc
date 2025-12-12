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

//virtual machine
char *data;

//lexical parse
int64 *symbals,*current_table;



int next(void);
int syntax(void);





#endif