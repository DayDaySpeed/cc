//win64,align 8 byte
//这个虚拟机用一个寄存器和栈来实现运算
//虚拟机也可以不用寄存器，仅靠栈式内存来实现运算
#include "defs.h"
#include "virtualmachine.h"



//virtual machine
int eval(){
    int64 op,     //opcode
        *tmp;   

        while(1){
            op = *pc++;     //get next opcode

            switch(op){
                case IMM:   {ax = *pc++;}                                       break;
                case LC:    {ax = *(char *)ax;}                                 break;
                case LI:    {ax = *(int *)ax;}                                  break;  //指针指向的地方取4字节，赋值给ax
                case SC:    {*(char *)*sp++ = ax;}                              break;  //将栈指向的内容解析成指针，该指针指向的地方赋值为ax（1个字节）
                case SI:    {*(int *)*sp++ = ax;}                               break;  //将栈指向的内容解析成指针，该指针指向的地方赋值为ax（4个字节）
                case PUSH:  {*--sp = ax;}                                       break;
                case JMP:   {pc = (int64 *)*pc;}                                break;
                case JEZ:   {pc = ax ? pc + 1 : (int64 *)*pc;}                  break;
                case JNZ:   {pc = ax ? (int64 *)*pc : pc + 1;}                  break;
                case CALL:  {*--sp = (int64)(pc + 1); pc = (int64 *)*pc;}       break;//想将返回地址保存到栈中，然后跳转到目标函数地址
                case ADJ:   {sp = sp + *pc++;}                                  break;
                case ENT:   {*--sp = (int64)bp; bp = sp; sp = sp - *pc++;}      break;
                case LEV:   {sp = bp; bp = (int64 *)*sp++; pc = (int64 *)*sp++;}break;
                case LAA:   {ax = (int64)(bp + *pc++);}                         break;// load arg's address
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

 //allocate memory for virtual machine

    if(!(data = malloc(poolsize))){
        printf("could not malloc (%d) for data segment",poolsize);
    }

    //init memory





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