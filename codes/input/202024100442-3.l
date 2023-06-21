%{
    #include <string.h>
    //行数
    int lineNum = 1;
    int wordsCnt = 0;
    //标识符表
    //可能是编译器版本的原因，这里不能写成id[][20]，否则字符串复制的时候会出错
    char id[20][20]= {""};
    int idNum = 0;
    int isIdExist(char *);
%}
emptyWords [\t\n" "]
annotation (\/\*.*\*\/)|(\/\/.*\n)
digit [0-9]
letter [a-zA-Z_]
keyWords "main"|"if"|"then"|"else"|"while"|"do"|"repeat"|"until"|"for"|"from"|"to"|"step"|"switch"|"of"|"case"|"default"|"return"|"integer"|"real"|"char"|"bool"|"and"|"not"|"mod"|"red"|"write"
id {letter}({letter}|{digit})*
integer ({digit})+
float {digit}+\.({digit}+)
operas "<="|">="|"!="|"="|"+"|"-"|"*"|"/"|"<"|">"
delimiters ","|";"|":"|"{"|"}"|"["|"]"|"("|")"

%%
{emptyWords} {//忽略空白符
    if(strcmp(yytext,"\n") == 0){
        //统计行数
        lineNum++; 
    }
}
{annotation} {//忽略注释
    printf("第%d行存在注释\n",lineNum);
}
{keyWords} {//统计关键字（关键字采用一个单词种别编码，通过属性值区分
    printf("<1,keyWords,'%s',->\n",yytext);wordsCnt++;
}
{id} {//统计标识符
    printf("<4,id,'%s','%d'>\n",yytext,isIdExist(yytext));wordsCnt++;
}
{operas} {//统计运算符
    printf("<2,operators,'%s',->\n",yytext);wordsCnt++;
}
{delimiters} {//统计分隔符
    printf("<3,delimiters,'%s',->\n",yytext);wordsCnt++;
}
{integer} {//统计整数常量
    printf("<5,num,int,'%d'>\n",atoi(yytext));wordsCnt++;
}
{float} {
    printf("<5,num,float,'%.2lf'>\n",strtod(yytext,NULL));wordsCnt++;
}
. {
    wordsCnt++;
}
%%
void main(){
    printf("========================\n");
    printf("=     LEX 词法分析器   =\n");
    printf("========================\n");
    printf("单词符号输出格式:<单词种别(整数),单词符号的属性>\n");
    printf("单词种别：\n");
    printf("========================\n");
    printf("=      1.keyWords      =\n");
    printf("=      2.operators     =\n");
    printf("=      3.delimiters    =\n");
    printf("=      4.id            =\n");
    printf("=      5.num           =\n");
    printf("========================\n");
    printf("请输入文件名:");
    char fileName[30];
    scanf("%s",fileName);
    //strcpy(fileName,"Quick_Sort.txt");
    yyin = fopen(fileName,"r");
    if(yyin == NULL){
        printf("文件%s打开失败！\n",fileName);return;
    }
    yylex();
    fclose(yyin);
    printf("=========================\n");
    printf("输入文件信息统计:\n");
    printf("输入文件名:%s\n",fileName);
    printf("文件行数：%d\n",lineNum);
    printf("文件词数（不包括空白符）:%d\n",wordsCnt);
    printf("=========================\n");
    system("pause");
}
int yywrap() 
{ 
  return 1; 
}
int isIdExist(char *token){
    for(int i = 0; i < idNum; i++){
        if(strcmp(id[i],token) == 0){
            return i;
        }
    }
    //自己写的字符串复制
    // for(int i = 0; token[i] != 0; i++){
    //     id[idNum][i] = token[i];
    //     id[idNum][i+1] = '\0';
    // }
    strcpy(id[idNum],token);
    idNum++;
    return idNum-1;
}