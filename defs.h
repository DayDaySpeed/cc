#ifndef DEFS_H
#define DEFS_H
#define int64 long long

int line;

//alloc memmory size
#define poolsize 1024*1024

//file
char *startcharpointer;
char *currentcharpointer;

//virtual machine
char *data;

//lexical parse
int64 *symbals,*current_table;


int next(void);






#endif