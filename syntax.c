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
#include "virtualmachine.h"
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
        current_table[Type] = INT;//default int
        current_table[Value] = number;
        //如果是，则下一个token
        if(token == ','){
            next();
        }
    }
}

void function_declaration(){
    //int func(){}
    match('(');
    function_parameter();
    match(')');
    match('{');
    function_body();

}

void function_parameter(){
    int type;
    int arg_params = 1;
    while(token != ')'){
        if(token == Int){
            type = INT;
            match(Int);
        }else if(token == Char){
            type = CHAR;
            match(Char);
        }
        while(token == Mul){
            type += PTR;
            match(Mul);
        }

        if(token != ID){
            printf("function_parameter error in line (%d): unexpected token, should be (ID) here, but got (%d)\n", line,token);
            exit(-1);
        }
        if(current_table[Class] == Loc){
            printf("function_parameter error in line (%d), identifier (%d) has been definded, its Class is (%d)\n",line, current_table[Name], current_table[Class]);
            exit(-1);
        }
        match(ID);

        current_table[Class] = Loc;
        current_table[Type] = type;
        current_table[Value] = arg_params++;

        if(token == ','){
            match(',');
        }
    }

    index_bp = arg_params;
}

void function_body(){
    int type;
    int local_params = -1;
    while(token == Int || token == Char){
        type = (token == Int) ? INT: CHAR;
        match(token);

        while(token != ';'){
            while(token == Mul){
                type += PTR;
                match(Mul);
            }
            if(token != ID){
                printf("function_body error in line (%d): unexpected token, should be (ID) here, but got (%d)\n", line,token);
                exit(-1);
            }
            if(current_table[Class] == Loc){
                printf("function_body error in line (%d), identifier (%d) has been definded, its Class is (%d)\n",line, current_table[Name], current_table[Class]);
                exit(-1);
            }
            match(ID);

            current_table[Class] = Loc;
            current_table[Type] = type;
            current_table[Value] = local_params--;

            if(token == ','){
                match(',');
            }
        }
        match(';');//分号后，下一个
    }

    *++text = ENT;
    *++text = -local_params;

    //statement
    while(token != '}'){
        statement();
    }

    *++text = LEV;
}









void global_declaration(){
    int type;
    int tmp;

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
        type = INT;
        match(Int);
    }else if(token == Char){
        match(Char);
        type = CHAR;
    }

    while(token != ';' && token != '}'){
        //解析int a,b; int ****c;

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
        if(current_table[Gclass]){
            printf("glabal declaration error in line (%d), identifier (%d) has been definded, its Class is (%d)\n",line, current_table[Name], current_table[Class]);
            exit(-1);
        }

        match(ID);
        current_table[Type] = type;

        if(token == '('){
            //若是（，则为函数名
            current_table[Gclass] = Func;
            current_table[Gvalue] = (int64)(text + 1);
            function_declaration();//具体处理函数逻辑
        }else{
            //不是（，就代表是全局变量
            current_table[Gclass] = Glo;
            current_table[Gvalue] = (int64)data;
            data += 1;
        }

        if(token == ','){
            match(',');
        }
    }
    next();
}

//解析语句
void expression(int level){//优先级用于解析二元运算符
    int current_type;

    if(!token){

    }

    //+-----------------------------------------------+
    //一元运算
    if(token == Num){
        match(Num);
        *++text = IMM;
        *++text = currentnumber;
        etype = INT;
    }
    else if(token == '"'){
        *++text = IMM;
        *++text = currentstringpointer;
        match('"');
        while(token == '"'){match('"');};//越过连续的",已经记录好了字符串开头地址，后面再补个0
        *data++ = 0;
        etype = PTR;
    }
    else if(token == Sizeof){
        match(Sizeof);
        match('(');
        etype = default_type;

        if(token == Int){
            match(Int);
            etype = INT;
        }else if(token == Char){
            match(Char);
            etype = CHAR;
        }
        while(token = Mul){
            match(Mul);
            etype += PTR;
        }

        match(')');

        *++text = IMM;
        *++text = (etype == CHAR)? sizeof(char) : sizeof(int);

        etype = default_type;
    }
    else if(token == ID){
        match(ID);

        //标识符后面接括号，说明是函数
        if(token == '('){
            match('(');
            int args_num = 0;
            while(token != ')'){
                expression(Assign);
                //将函数参数压栈
                *++text = PUSH;
                args_num++;
                //消耗，处理下一个参数
                if(token == ','){
                    match(',');
                }
            }
            match(')');

            if(current_table[Class] == Sys){
                *++text = current_table[Value];
            }else if(current_table[Class] == Func){
                *++text = CALL;
                *++text = current_table[Value];
            }else{
                printf("");
                exit(-1);
            }

            if(args_num > 0){
                *++text == ADJ;
                *++text == args_num;
            }

            etype = current_table[Type];
        }

        //当前标识符表示单个字符，如a = 'a';
        else if(current_table[Class] == Num){
            *++text = IMM;
            *++text = current_table[Value];
            etype = INT;
        }

        //区分ID是局部变量还是全局变量
        else{
            //若ID是局部变量，就用局部变量，否则用全局变量
            if(current_table[Class] == Loc){
                *++text = LAA;
                *++text = index_bp - current_table[Value];
            }else if(current_table[Gclass] == Glo && current_table[Class] == 0){
                *++text = IMM;
                *++text = current_table[Value];
            }else{
                printf("");
                exit(-1);
            }

            etype = current_table[Type];
            *++text = (current_table[Type] == CHAR)? LC:LI;
        }
    }

    else if(token == '('){
        match('(');
        if(token == Int || token == Char){
            //(int)
            current_type = (token == Char)? CHAR : INT;
            match(token);
            //(int *)
            while(token == Mul){
                match(Mul);
                current_type += PTR;
            }
            match(')');
            //(int *)a++
            expression(Inc);   
            etype = current_type;
        }else{
            //(expr)
            expression(Assign);
            match(')');
        }


    }
    else if(token == Mul){
        //*p
        match(Mul);

        //*p++/*p--,相当于*(p++)/*(p--)
        expression(Inc);

        if(etype <= PTR){
            printf("Not a valid pointer type\n");
            exit(-1);
        }

        etype -= PTR;
        *++text = (etype == CHAR) ? LC : LI;

    }
    else if(token == And){
        //&p
        match(And);
        //&(p++)
        expression(Inc);
    }
    else if(token == ADD){
        //+a
        match(Add);
        //+a[]
        expression(Brak);

    }
    else if(token == Sub){
        //-5
        match(Sub);
        if(token == Num){
            *++text = IMM;
            *++text = -currentnumber;
            match(Num);
        }else{
            //-a
            *++text = IMM;
            *++text = -1;
            *++text = PUSH;
            //-a[]
            expression(Brak);
            *++text = MUL;
        }
    }

    else if(token == Inc || token == Dec){
        int tk = token;
        //++p
        match(token);
        //++p[]
        expression(Brak);
        if(*text == LC){
            *text = PUSH;
            *++text = LC;
        }
        else if(*text == LI){
            *text = PUSH;
            *++text = LI;
        }else{
            printf("error in line (%d) : ++p[]\n", line);
        }
        *++text = PUSH;
        *++text = IMM;
        *++text = etype > PTR ? sizeof(int) : sizeof(char); 
        *++text = (tk == Inc)? ADD : SUB;
        *++text = (etype == CHAR)? SC : SI;
    }

    else if(token == '!'){
        //!a

        //!a++
    }
    else if(token == '~'){
        //~a

        //~a++
    }else{

        printf();
        exit(-1);


    }
    //+----------------------------------------------------------+
    //二元运算，需要有左值

    while(token > level){
        current_type = etype;


        if(token == Assign){
            match(Assign);
            //左值必须是变量
            if(*text != LC || *text != LI){
                printf();
                exit(-1);
            }
            *text == PUSH;//此刻ax中是变量地址，不再取变量的值，而是将变量地址压入栈
            
            //处理等号右边
            expression(Assign);
            *++text = (etype == CHAR) ? SC : SI;
        }

        else if(token == Cond){
            int64* addr;
            //int a = a > b? a: b;
            match(Cond);

            *++text = JEZ;
            addr = ++text;
            //解析a部分的表达式
            expression(Assign);

            if(token != ':'){
                printf("");
                exit(-1);
            }
            match(':');

            *addr = (int64)(text + 3);
            *++text = JMP;
            addr = ++text;

        }

        //Lor
        else if(token == Lor){
            //a || b
            match(Lor);
        }
        //Lan
        else if(token == Lan){
            //a && b
            match(Lan);

        }

        else if(token == Or){
            //a | b
            match(Or);
        }

        else if(token == Xor){
            //a ^ b
            match(Xor);
        }

        else if(token == And){
            //a & b
            match(And);
        }

        else if(token == Eq){
            //a == b
            match(Eq);
        }

        else if(token == Ne){
            //a != b
            match(Ne);
        }

        else if(token == Lt){
            // a < b
            match(Lt);
        }

        else if(token == Gt){
            // a > b
            match(Gt);
        }

        else if(token == Le){
            // a <= b
            match(Le);
        }

        else if(token == Ge){
            // a >= b
            match(Ge);
        }

        else if(token == Shl){
            // a << b
            match(Shl);
        }

        else if(token == Shr){
            // a >> b
            match(Shr);
        }

        else if(token == Add){
            // a + b
            match(Add);
        }
        
        else if(token == Sub){
            // a - b
            match(Sub);
        }

        else if(token == Inc || token == Dec){

            if(*text = LC){
                *text = PUSH;
                *++text = LC;
            }else if(*++text = LI){
                *text = PUSH;
                *++text = LI;
            }else{
                printf();
                exit(-1);
            }

            //修改变量值
            *++text = PUSH;
            *++text = IMM;
            *++text = (etype > PTR) ? sizeof(int) : sizeof(char); 
            *++text = (token == Inc) ? ADD : SUB;
            *++text = (etype == CHAR) ? SC : SI;
            //恢复原始值
            *++text = PUSH;
            *++text = IMM;
            *++text = (etype > PTR) ? sizeof(int) : sizeof(char); 
            *++text = (token == Inc) ? SUB : ADD;
            match(token);

        }

        else if(token == Brak){
            //int *a
            //a[10]
            match(Brak);
            *++text = PUSH;
            expression(Assign);
            match(']');

            if(current_type <= PTR){
                printf("");
                exit(-1);
            }
            *++text = PUSH;
            *++text = IMM;
            *++text = sizeof(Int);
            *++text = MUL;
            *++text = ADD;

            etype = current_type - PTR;//数组的最终类型
            *++text = (etype == CHAR) ? LC: LI;
        }
    }
}

void statement(){
    //          |   JEZ         |
    //          |   addr_else   |
    //          |   statement1  |
    //          |   JMP         |
    //          |   addr_end    |
    //else----->|   statement2  |
    //end------>|   end         |

    if(token == If){
        int64 *addr_else,*addr_end;
        match(If);
        match('(');
        expression(Assign);
        match(')');

        *++text = JEZ;
        addr_else = ++text;
        statement();

        //如果有else
        if(token == Else){
            match(Else);
            *addr_else = (int64)(text + 3);
            *++text = JMP;
            addr_end = ++text;
            statement();
        }else{
        //没有else
            *addr_else = (int64)(text + 1);
        }

        *addr_end = (int64)(text + 1);
    }
    else if(token == While){
        //a:    evaluate cond
        //      JEZ
        //      b
        //      statement
        //      JMP
        //      a
        //b:
        int64 *a,*b;
        match(While);
        //记录判断条件指令的开始位置
        a = ++text;
        match('(');
        expression(Assign);
        match(')');
        *++text = JEZ;
        b = ++text;
        statement();
        *++text = JMP;
        *++text = (int64)a;
        *b = (int64)(text + 1);
    }
    else if(token == Return){
        match(Return);
        if(token != ';'){
            expression(Assign);
        }
        match(';');

        *++text = LEV;
    }
    else if(token == ';'){
        match(';');
    }
    else{
        expression(Assign);
        match(';');
    }


}