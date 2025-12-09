#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//我规定，64位中，数组数据都以8字节存储，尽管会浪费很多空间
#include "lexical.h"
#include "defs.h"



int next(void){


    char currentchar;

    int hash;



  while((currentchar = *currentcharpointer) != 0){
        ++currentcharpointer;

        //start parse token

        if(currentchar == '\n'){
            ++line;
        }else if(currentchar == '#'){
            //donothing skip
            while(*currentcharpointer != '\n' && *currentcharpointer != 0){
                ++currentcharpointer;
            }
        }

        //parse identifier
        else if((currentchar >= 'a' && currentchar <= 'z')||(currentchar >= 'A' && currentchar <= 'Z')||currentchar == '_'){
            //start parse identifier
            //look ahead one char,the start of a string
            startcharpointer = --currentcharpointer;
            hash = currentchar;

            while((*currentcharpointer >= 'a' && *currentcharpointer <= 'z') || (*currentcharpointer >= 'A' && *currentcharpointer <= 'Z') || (*currentcharpointer >= '0' && *currentcharpointer <= '9') || (*currentcharpointer == '_')){
                    hash = hash * 31 + *currentcharpointer;
                    ++currentcharpointer;
            }

            //look for existing identifier from symbal table
            current_table = symbals;
            while(current_table[Token]){
                if(current_table[Hash] == hash && !memcmp((char *)current_table[Name], startcharpointer, currentcharpointer - startcharpointer)){
                    //find
                    return current_table[Token];
                }
                //not find,next symbal table
                current_table += IDSIZE;
            }

            //store new identifier
            current_table[Name] = (int64)startcharpointer;//存储标识符在文件中的地址
            //printf("current_table[Name] = %d\n",current_table[Name]);
            current_table[Hash] = hash;
            current_table[Token] = ID;
            return ID;
        }

        //parse number
        else if(currentchar >= '0' && currentchar <= '9'){
            int currentnumber;
            //dec(91) hex(0x78) oct(069)
            currentnumber = currentchar - '0';
            //dex
            if(currentnumber > 0){
                while(*currentcharpointer >= '0' && *currentcharpointer <= '9'){
                    currentnumber = currentnumber * 10 + *currentcharpointer++ - '0';
                }
            }else
            if(currentnumber == 0){
                    //hex
                if(*currentcharpointer == 'x' || *currentcharpointer == 'X'){
                    currentchar = *++currentcharpointer;
                    while((currentchar >= '0' && currentchar <= '9') || (currentchar >= 'a' && currentchar <= 'f') || (currentchar >= 'A' && currentchar <= 'F')){
                        currentnumber = currentnumber * 16 + (currentchar & 15) + (currentchar > 'A' ? 9 : 0);
                        currentchar = *++currentcharpointer;
                    }
                }else{
                    //oct
                    while(*currentcharpointer >= '0' && *currentcharpointer <= '7'){
                        currentnumber = currentnumber * 8 + *currentcharpointer++ - '0';
                    }
                }
            }  
            return Num;
        }

        //parse string
        else if(currentchar == '"' || currentchar == '\''){
            //if a string or char start with " or '
            //startcharpointer = data;//指向数据段的开头
            char samechar = currentchar;
            while(*currentcharpointer != 0 && *currentcharpointer != samechar){
                currentchar = *currentcharpointer++;
                if(currentchar == '\\'){
                    currentchar = *currentcharpointer++;
                    if(currentchar == 'n'){currentchar = '\n';}
                }

                *data++ = currentchar;       
            }
            *data++ = 0;
            ++currentcharpointer;
            //printf("%s\n",startcharpointer);
            if(samechar == '\''){return Num;}
            return currentchar;
        }


        //注释，仅支持//
        else if(currentchar == '/'){
            if(*currentcharpointer == '/'){
                //一直自增currentcharpointer,直到/n
                while(*currentcharpointer != 0 && *currentcharpointer != '\n'){
                    ++currentcharpointer;
                }
            }else{
                //如果/后面不是/，则是除号
                return Div;
            }
        }

        //others
        else if(currentchar == '='){
            //需要解析是== 还是 =
            if(*currentcharpointer == '='){
                //==
                ++currentcharpointer;
                return Eq;
            }else{
                //=
                return Assign;
            }
        }
        else if(currentchar == '+'){
            //parse '+' or '++'
            if(*currentcharpointer == '+'){
                ++currentcharpointer;
                return Inc;
            }else{
                return Add;
            }
        }
        else if(currentchar == '-'){
            //parse '-' or '--'
            if(*currentcharpointer == '-'){
                ++currentcharpointer;
                return Dec;
            }else{
                return Sub;
            }
        }
        else if(currentchar == '!'){
            //parse '!='
            if(*currentcharpointer == '='){
                ++currentcharpointer;
                return Ne;
            }
        }
        else if(currentchar == '<'){
            //parse '<' or '<<' or '<='
            if(*currentcharpointer == '<'){
                ++currentcharpointer;
                return Shl;
            }else if(*currentcharpointer == '='){
                ++currentcharpointer;
                return Le;
            }else{
                return Lt;
            }
        }
        else if(currentchar == '>'){
            //parse '>' or '>>' or '>=
             if(*currentcharpointer == '>'){
                ++currentcharpointer;
                return Shr;
            }else if(*currentcharpointer == '='){
                ++currentcharpointer;
                return Ge;
            }else{
                return Gt;
            }
        }
        else if(currentchar == '|'){
            if (*currentcharpointer == '|'){
                ++currentcharpointer;
                return Lor;
            }else{
                return Or;
            }
        }
        else if(currentchar == '&'){
            if(*currentcharpointer == '&'){
                ++currentcharpointer;
                return Lan;
            }else{
                return And;
            }
        }
        else if(currentchar == '^'){
            return Xor;
        }
        else if(currentchar == '%'){
            return Mod;
        }
        else if(currentchar == '*'){
            return Mul;
        }
        else if(currentchar == '['){
            return Brak;
        }
        else if(currentchar == '?'){
            return Cond;
        }
        else if(currentchar == '~' || currentchar == ';' || currentchar == '{' || currentchar == '}' || currentchar == '(' || currentchar == ')' || currentchar == ']' || currentchar == ',' || currentchar == ';'){
            return currentchar;
        }
    }
    return currentchar;
}