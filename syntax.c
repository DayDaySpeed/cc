/*
<expr> ::= <term> <expr_tail>
<term> ::= <factor> <term_tail>
<factor> ::= (<expr>) |Num

<expr_tail> = + <term> <expr_tail> | - <term> <expr_tail> | <empty>
<term_tail> = * <factor> <term_tail> | / <factor> <term_tail> | <empty>
*/
#include "defs.h"
#include "lexical.h"
#include "syntax.h"
//消耗token
void match(int tk){
    if(token != tk){
        printf("unexpected token error: expected token is (%d) , but got token is (%d)\n",tk,token);
        exit(-1);
    }
    token = next();
}


//提供对外调用
int syntax(){
    return expr();
}



//syntax number
int expr(){
    return expr_tail(term());
}
int term(){
    return term_tail(factor());
}
int factor(){
    int value = 0;
    if(token == '('){
        match('(');
        value = expr();
        match(')');
    }else{
        value = currentnumber;
        match(Num);
    }
    return value;
}
int expr_tail(int value){
    if(token == Add){
        match(Add);
        return expr_tail(value + term());
    }else if(token == Sub){
        match(Sub);
        return expr_tail(value - term());
    }else{
        return value;
    }
}
int term_tail(int value){
    if(token == Mul){
        match(Mul);
        return term_tail(value * factor());
    }else if(token == Div){
        match(Div);
        return term_tail(value / factor());
    }else{
        return value;
    }
}


//syntax enum
void enum_declaration(){
    //for example: enum color {red = 1, green = 2};
    int number = 0;
    while(token != '}'){
        //检查id
        if(token != ID){
            printf("enum error in line (%d): unexpected identifier (%d)\n", line, token);
            exit(-1);
        }
        //next token
        next();
        if(token == Assign){
            next();
            //检查num
            if(token != Num){
                printf("enum error in line (%d): unexpected num (%d)\n", line, token);
                exit(-1);
            }
            number = currentnumber;
            next();
        }
        //ID已经在词法分析过程中记录在符号表中
        current_table[Class] = Num;
        current_table[Type] = Int;//default int
        current_table[Value] = number;
        //如果是，则下一个token
        if(token == ','){
            next();
        }
    }
}
void global_declaration(){
    int type;
    int tmp;
    int current_type;
    if(token == Enum){
        match(Enum);
        if(token != '{'){
            if(token != ID){
                printf("enum error in line (%d): unexpected format, enum error-> _ <-here {},should be (ID), but got (%d)\n", line, token);
            }
            match(ID);
        }else{
            match('{');
            enum_declaration();
            match('}');
        }
        match(';');
        return;
    }

    if(token == Int){
        match(Int);
    }else if(token == Char){
        match(Char);
        current_type = CHAR;
    }

    while(token != ';' && token != '}'){
        type = current_type;
        //解析指针类型
        while(token == Mul){
            match(Mul);
            type += PTR;
        }
        if(token != ID){
            printf("glabal declaration error in line (%d), unexpected token, should be ID here, but got (%d)\n",line, token);
            exit(-1);
        }
        //查表看当前ID的类别是否已经存在
        if(current_table[Class]){
            printf("glabal declaration error in line (%d), identifier (%d) has been definded, its Class is (%d)\n",line, current_table[Name], current_table[Class]);
            exit(-1);
        }

        match(ID);
        current_table[Type] = type;

        if(token == '('){
            current_table[Class] = Func;
            current_table[Value] = (int64)(text + 1);
        }
    }
}