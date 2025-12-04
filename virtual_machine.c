#include <fcntl.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define int64 long long


int token;
char *src,*old_src;
int line;
int poolsize;

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

//virtual machine
int eval(){
    int64 op,     //opcode
        *tmp;   

        while(1){
            op = *pc++;     //get next opcode

            switch(op){
                case IMM:   {ax = *pc++;}                                       break;
                case LC:    {ax = *(char *)ax;}                                 break;
                case SC:    {*(char *)*sp++ = ax;}                              break;  //将栈指向的内容解析成指针，该指针指向的地方赋值为ax（1个字节）
                case LI:    {ax = *(int *)ax;}                                  break;  //指针指向的地方取4字节，赋值给ax
                case SI:    {*(int *)*sp++ = ax;}                               break;  //将栈指向的内容解析成指针，该指针指向的地方赋值为ax（4个字节）
                case PUSH:  {*--sp = ax;}                                       break;
                case JMP:   {pc = (int64 *)*pc;}                                break;
                case JEZ:   {pc = ax ? pc + 1 : (int64 *)*pc;}                  break;
                case JNZ:   {pc = ax ? (int64 *)*pc : pc + 1;}                  break;
                case CALL:  {*--pc = (int64)(pc + 1); pc = (int64 *)*pc;}       break;//想将返回地址保存到栈中，然后跳转到目标函数地址
                case ADJ:   {sp = sp + *pc++;}                                  break;
                case ENT:   {*--sp = (int64)bp; bp = sp; sp = sp - *pc++;}      break;
                case LEV:   {sp = bp; bp = (int64 *)*sp++; pc = (int64 *)*sp++;}break;
                case LAA:   {ax = (int64)(bp + *pc++);}                         break;
                //Arithmetic operations
                case OR:   {ax = *sp++ | ax;}                                   break;
                case XOR:  {ax = *sp++ ^ ax;}                                   break;
                case AND:  {ax = *sp++ & ax;}                                   break;
                case EQ:   {ax = *sp++ == ax;}                                  break;
                case NE:   {ax = *sp++ != ax;}                                  break;
                case LT:   {ax = *sp++ < ax;}                                   break;
                case LE:   {ax = *sp++ <= ax;}                                  break;
                case GT:   {ax = *sp++ > ax;}                                   break;
                case GE:   {ax = *sp++ >= ax;}                                  break;
                case ADD:  {ax = *sp++ + ax;}                                   break;
                case SUB:  {ax = *sp++ - ax;}                                   break;
                case MUL:  {ax = *sp++ * ax;}                                   break;
                case DIV:  {ax = *sp++ / ax;}                                   break;
                case MOD:  {ax = *sp++ % ax;}                                   break;
                //system function
                //不用bp，只用sp
                //只用到了参数，没有局部变量，也没有调用者
                case OPEN:  { ax = open((char *)sp[1],sp[0]); }                 break;//将文件描述符fd写入ax,   [sp[1]是文件名字符串，sp[0]是文件权限]stack
                case CLOS:  { ax = *sp;}                                        break;
                case READ:  { ax = read(sp[2], (char *)sp[1], sp[0]);}          break;//read(fd(int),buf(char *),size(int))
                case PRTF:  { tmp = sp + pc[1]; ax = printf((char *)tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]);}break;//硬编码了六个参数
                case MALC:  { ax = (int64)malloc(*sp);}                         break;
                case MSET:  { ax = (int64)memset((char *)sp[2], sp[1], sp[0]);} break;//memset(void *_Dst,int _Val,size_t _Size)
                case MCMP:  { ax = memcmp((char *)sp[2], (char *)sp[1], sp[0]);}break;//memcmp(const void *_Buf1, const void *_Buf2, size_t _Size);逐字节比较,第一个字节的比较结果决定返回值
                case EXIT:  { printf("exit (%d)\n", *sp); return *sp;}
                default:    printf("%d\n",*sp);return -1;
            }
        }
    return 0;
}


int main(){

    poolsize = 1024 * 1024; // 或者更大


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
    bp = sp = (int64 *)((int64)stack + poolsize);
    ax = 0;


    int i = 0;

    text[i++] = IMM;
    text[i++] = 10;
    text[i++] = PUSH;
    text[i++] = IMM;
    text[i++] = 20;
    text[i++] = ADD;
    text[i++] = PUSH;
    text[i++] = EXIT;
    pc = text;

    return eval();
}