#ifndef LEXICAL_H
#define LEXICAL_H


enum{
    Num=128,    //数字常量        
    Func,       //函数
    Sys,        //系统调用
    Glo,        //全局变量
    Loc,        //局部变量
    ID,         //标识符
    Char,Else,Enum,If,Int,Return,Sizeof,While,  //关键字
    Assign,Cond,Lor,Lan,Or,Xor,And,Eq,Ne,Lt,Gt,Le,Ge,Shl,Shr,Add,Sub,Mul,Div,Mod,Inc,Dec,Brak//运算符，按优先级顺序排序
};

//identifier fields
enum{Token,Hash,Name,Type,Class,Value,Gtype,Gclass,Gvalue,IDSIZE};
//+-----------------------------------------------------+
//|Token|Hash|Name|Type|Class|Value|Gtype|Gclass|Gvalue|
//+------------------------IDSIZE-----------------------+




#endif