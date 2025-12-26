#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHIEN_H


//An enum is an int constant
enum{
    IMM,        // a number
    LC,         // load a char
    LI,         // load a int
    SC,         // save a char
    SI,         // save a int
    JMP,        // jump to address
    PUSH,       // push the value of ax
    JEZ,        // jump if ax is zero
    JNZ,        // jump if ax is not zero
    CALL,       // call a function
    ENT,        // (ENTER) create a new stack frame for function entry
    LEV,        // (LEAVE) discard the current stack frame and return from function
    ARG,        // access function argument from caller's stack frame
    ADJ,        // (ADJUST) remove arguments from frame
    LAA,        // load argument's adress

    OR,
    XOR,
    AND,
    EQ,
    NE,
    LT,
    LE,
    GT,
    GE,
    SHL,
    SHR,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,

    EXIT,
    OPEN,
    CLOS,
    READ,
    PRTF,
    MALC,
    MSET,
    MCMP

};


/*
                +-----------+
                |argc       |
                |argv       |
                |address    |<-----调用main函数的调用者的下一条语句地址
stack frame---->|old bp     |<-----new bp
                |local var1 |
                |local var..|
                +-----------+
                |arg1       |
                |arg..      |
                |address    |<-----main函数调用完fun1()后，下一条语句的地址
stack frame---->|old bp     |<-----new bp
                |local var1 |
                |local var..|
                +-----------+
                |           |<-----sp
*/
#endif