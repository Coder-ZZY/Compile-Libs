#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//符号的最大长度
#define MAX_SIZE 20
//单词符号的最大长度
#define MAX_SYMLEN 50
/*必须使用全局变量keyWords,opera,delimiters,id，digitals
几乎每个函数调用时都需要访问这些变量。
若不使用,将会降低程序的可读性以及需要在各个函数之间频繁地传递变量（以空间换取时间）*/
//关键字
char keyWords[][27]= {"main","if","then","else","while","do","repeat","until","for","from","to","setp","switch","of",
                      "case","default","return","integer","real","char","bool","and","or","not","mod","read","write"
                     };
//<main>的单词种别为1，<write>为27
//运算符
char opera[][10]= {"=","+","-","*","/","<","<=",">",">=","!="}; //< = >为28，< != >为37
//分隔符
char delimiters[][9]= {",",";",":","{","}","[","]","(",")"}; //< , >为38，< ) >为46
//标识符表   47
char id[][20]= {""};
//常数表  48
char digitals[][20]= {""};

//判断关键字还是标识符
void isKeyWordOrID(char *token,int*idNump) {
	//循环匹配关键字
	for(int i = 0; i < 27; i++) {
		if(strcmp(token,keyWords[i]) == 0) {
			printf("<%d,%s,%s,->\n",i+1,keyWords[i],keyWords[i]);
			return;
		}
	}
	//执行到这，说明是标识符
	//查看标识符是否在标识符表中
	for(int i = 0; i < *idNump; i++ ) {
		if(strcmp(token,id[i]) == 0){
			printf("<47,id,%s,%d>\n",id[i],i);		
		return;}
	}
	//标识符不存在
	strcpy(id[*idNump],token);
	printf("<47,id,%s,%d>\n",id[*idNump],*idNump);
	(*idNump)++;
	return;
}
//判断是否为数字 
bool isDigit(char ch) {
	if(ch >= '0' && ch <= '9') {
		return true;
	} else	return false;
}
//插入常数 
//insertDigit(token,digitalLen)
void insertDigit(char* token, int* digitalLen){
	for(int i = 0; i < *digitalLen; i++){
		if(strcmp(digitals[i],token) == 0){
			printf("<48,NUM,%s,%d>\n",token,i);
			return;
		}
	}
	strcpy(digitals[*digitalLen],token);(*digitalLen)++;
	printf("<48,NUM,%s,%d>\n",token,*digitalLen);
	return;
}
//判断是否为注释还是除号
bool isDividOrAnno(FILE *fp,char *token,int *lineNumP){
	char prech;
	char ch = fgetc(fp);
	if(ch == '/'){	//单行注释 
		while(ch != '\n'){
			ch = fgetc(fp);
		}
		(*lineNumP)++;
		return true;
	}
	else if(ch == '*'){	/*段落注释*/ 
		int lineNumPre = *lineNumP;
		while( !feof(fp) ){
			prech = ch;
			ch = fgetc(fp);
			if(ch == '\n'){
				(*lineNumP)++;
			}else if(prech == '*'&&ch=='/'){
				return true;
			}
		}
		if(feof(fp)){
			printf("错误！第%d行段落注释缺少*/！！\n",lineNumPre);return false;
		}
	}
	else if(ch >= '0'&&ch <= '9'){ //除号 
		//指针回退一个 
		fseek(fp,-1,SEEK_CUR);
		printf("<32,/,/,->\n");
		return true; 
	}else{//exception：既不是注释也不是除号 
		printf("错误!在第%d行！\n",*lineNumP); 
	}
}
//其他分隔符或运算符
void isOthers(FILE *fp,char *token){
	char tmp; 
	//双字符的需要再次读入一个字符 
	if(token[0] == '<'){
		tmp = fgetc(fp);
		if(tmp == '='){
			printf("<34,%c%c,%c%c,->\n",token[0],tmp,token[0],tmp);return;
		}
		else{
			fseek(fp,-1,SEEK_CUR);//回退一个字符
			printf("<33,%c,%c,->\n",token[0],token[0]);return;
		}
	}
	else if(token[0] == '>'){
		tmp = fgetc(fp);
		if(tmp == '='){
			printf("<36,%c%c,%c%c,->\n",token[0],tmp,token[0],tmp);return;
		}
		else{
			fseek(fp,-1,SEEK_CUR);//回退一个字符
			printf("<35,%c,%c,->\n",token[0],token[0]);return;
		}
	}else if(token[0] == '!'){
		tmp = fgetc(fp);
		if(tmp == '='){
			printf("<37,%c%c,%c%c,->\n",token[0],tmp,token[0],tmp);return;
		}
		else{
			fseek(fp,-1,SEEK_CUR);//回退一个字符
			return;
		}
	}
	//遍历运算符 (只需要遍历0-3） 
	for(int i = 0;i < 4; i++ ) {
		if(strcmp(token,opera[i]) == 0){
			printf("<%d,%c,%c,->\n",i+28,token[0],token[0]);
			return;
		}
	}
	//遍历分隔符
	for(int i = 0; i < 9; i++){
		if(strcmp(token,delimiters[i]) == 0){
			printf("<%d,%c,%c,->\n",i+38,token[0],token[0]);return;
		}
	}
	return;
} 
int fileSC(char *fileName, char *token,int *idNump,int *digitalLen) {
	FILE *IN;
	if((IN=fopen(fileName,"r")) == NULL) {
		printf("无法打开%s文件",fileName);
		return -1;
	}
	//行数
	int line_Num = 1;
	//token下标
	int i = 0;
	//判断是否为实数
	bool isFloat = false; 
	char ch;
	//读取一个字符
	ch = fgetc(IN);
	while(!feof(IN)) {
		//跳过空白符并统计行数
		if(ch==' '||ch=='\t'||ch=='\n') {
			if(ch=='\n')
				line_Num++;
			ch=fgetc(IN);
		}else if( (ch >= 'a' && ch <= 'z') || ch == '_' ) {	//若读取的字符是字母或下划线_
			//ch赋值给token[i]后,i+1
			i = 0;
			token[i++] = ch;
			ch = fgetc(IN);
			while((ch>='a'&&ch<='z')||ch == '_'||(ch>='0'&&ch<='9')) {
				token[i++] = ch;
				ch = fgetc(IN);
			}
			//token字符串后需要添加终止符
			token[i] = '\0';
			//判断是关键字还是标识符
			isKeyWordOrID(token,idNump);
		} else if(ch>= '0'&&ch<='9') {
			i = 0;
			token[i++] = ch;
			ch = fgetc(IN);
			while(isDigit(ch)) {
				token[i++] = ch;
				ch = fgetc(IN);
			}
			//如果下一位为小数点
			if( ch == '.' ) {
				isFloat = true;
				token[i++]=ch;
				ch = fgetc(IN);
				while(isDigit(ch)) {
					token[i++] = ch;
					ch = fgetc(IN);
				}
			}
			token[i] = '\0';
			//printf("%s",token);
			if(!isFloat){
				//将整数转换成2进制字符串 
				itoa(atoi(token),token,2);
			}
			insertDigit(token,digitalLen);
			//识别运算符 
		} else if(ch == '/'){	//可能是注释，也有可能是除号 
			token[0] = ch;
			if(!isDividOrAnno(IN,token,&line_Num)){//exception处理 
				fclose(IN);
				return -1;
			}
			ch = fgetc(IN);
		}else{//其他情况 
			token[0] = ch;
			isOthers(IN,token);
			ch = fgetc(IN);
		}
		//重置token 
		memset(token,0,strlen(token));
	}
	fclose(IN);
}
int main() {
	//行数
	int lineNum;
	//标识符表中的数目
	int idNum = 0;
	//常数表中的数目
	int digitalLen = 0;
	//token字符数组，存放构成单词符号的字符串
	char token[MAX_SIZE];
	//文件指针
	char fileName[50];
	printf("hello"); 
//	system("pause");
	printf("输入文件名：\n");
	//strcpy(fileName,"F:\\Quick_Sort.txt"); 
	scanf("%s",fileName);
	fileSC(fileName,token,&idNum,&digitalLen);
	return 0;
}
