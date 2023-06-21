#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//���ŵ���󳤶�
#define MAX_SIZE 20
//���ʷ��ŵ���󳤶�
#define MAX_SYMLEN 50
/*����ʹ��ȫ�ֱ���keyWords,opera,delimiters,id��digitals
����ÿ����������ʱ����Ҫ������Щ������
����ʹ��,���ή�ͳ���Ŀɶ����Լ���Ҫ�ڸ�������֮��Ƶ���ش��ݱ������Կռ任ȡʱ�䣩*/
//�ؼ���
char keyWords[][27]= {"main","if","then","else","while","do","repeat","until","for","from","to","setp","switch","of",
                      "case","default","return","integer","real","char","bool","and","or","not","mod","read","write"
                     };
//<main>�ĵ����ֱ�Ϊ1��<write>Ϊ27
//�����
char opera[][10]= {"=","+","-","*","/","<","<=",">",">=","!="}; //< = >Ϊ28��< != >Ϊ37
//�ָ���
char delimiters[][9]= {",",";",":","{","}","[","]","(",")"}; //< , >Ϊ38��< ) >Ϊ46
//��ʶ����   47
char id[][20]= {""};
//������  48
char digitals[][20]= {""};

//�жϹؼ��ֻ��Ǳ�ʶ��
void isKeyWordOrID(char *token,int*idNump) {
	//ѭ��ƥ��ؼ���
	for(int i = 0; i < 27; i++) {
		if(strcmp(token,keyWords[i]) == 0) {
			printf("<%d,%s,%s,->\n",i+1,keyWords[i],keyWords[i]);
			return;
		}
	}
	//ִ�е��⣬˵���Ǳ�ʶ��
	//�鿴��ʶ���Ƿ��ڱ�ʶ������
	for(int i = 0; i < *idNump; i++ ) {
		if(strcmp(token,id[i]) == 0){
			printf("<47,id,%s,%d>\n",id[i],i);		
		return;}
	}
	//��ʶ��������
	strcpy(id[*idNump],token);
	printf("<47,id,%s,%d>\n",id[*idNump],*idNump);
	(*idNump)++;
	return;
}
//�ж��Ƿ�Ϊ���� 
bool isDigit(char ch) {
	if(ch >= '0' && ch <= '9') {
		return true;
	} else	return false;
}
//���볣�� 
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
//�ж��Ƿ�Ϊע�ͻ��ǳ���
bool isDividOrAnno(FILE *fp,char *token,int *lineNumP){
	char prech;
	char ch = fgetc(fp);
	if(ch == '/'){	//����ע�� 
		while(ch != '\n'){
			ch = fgetc(fp);
		}
		(*lineNumP)++;
		return true;
	}
	else if(ch == '*'){	/*����ע��*/ 
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
			printf("���󣡵�%d�ж���ע��ȱ��*/����\n",lineNumPre);return false;
		}
	}
	else if(ch >= '0'&&ch <= '9'){ //���� 
		//ָ�����һ�� 
		fseek(fp,-1,SEEK_CUR);
		printf("<32,/,/,->\n");
		return true; 
	}else{//exception���Ȳ���ע��Ҳ���ǳ��� 
		printf("����!�ڵ�%d�У�\n",*lineNumP); 
	}
}
//�����ָ����������
void isOthers(FILE *fp,char *token){
	char tmp; 
	//˫�ַ�����Ҫ�ٴζ���һ���ַ� 
	if(token[0] == '<'){
		tmp = fgetc(fp);
		if(tmp == '='){
			printf("<34,%c%c,%c%c,->\n",token[0],tmp,token[0],tmp);return;
		}
		else{
			fseek(fp,-1,SEEK_CUR);//����һ���ַ�
			printf("<33,%c,%c,->\n",token[0],token[0]);return;
		}
	}
	else if(token[0] == '>'){
		tmp = fgetc(fp);
		if(tmp == '='){
			printf("<36,%c%c,%c%c,->\n",token[0],tmp,token[0],tmp);return;
		}
		else{
			fseek(fp,-1,SEEK_CUR);//����һ���ַ�
			printf("<35,%c,%c,->\n",token[0],token[0]);return;
		}
	}else if(token[0] == '!'){
		tmp = fgetc(fp);
		if(tmp == '='){
			printf("<37,%c%c,%c%c,->\n",token[0],tmp,token[0],tmp);return;
		}
		else{
			fseek(fp,-1,SEEK_CUR);//����һ���ַ�
			return;
		}
	}
	//��������� (ֻ��Ҫ����0-3�� 
	for(int i = 0;i < 4; i++ ) {
		if(strcmp(token,opera[i]) == 0){
			printf("<%d,%c,%c,->\n",i+28,token[0],token[0]);
			return;
		}
	}
	//�����ָ���
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
		printf("�޷���%s�ļ�",fileName);
		return -1;
	}
	//����
	int line_Num = 1;
	//token�±�
	int i = 0;
	//�ж��Ƿ�Ϊʵ��
	bool isFloat = false; 
	char ch;
	//��ȡһ���ַ�
	ch = fgetc(IN);
	while(!feof(IN)) {
		//�����հ׷���ͳ������
		if(ch==' '||ch=='\t'||ch=='\n') {
			if(ch=='\n')
				line_Num++;
			ch=fgetc(IN);
		}else if( (ch >= 'a' && ch <= 'z') || ch == '_' ) {	//����ȡ���ַ�����ĸ���»���_
			//ch��ֵ��token[i]��,i+1
			i = 0;
			token[i++] = ch;
			ch = fgetc(IN);
			while((ch>='a'&&ch<='z')||ch == '_'||(ch>='0'&&ch<='9')) {
				token[i++] = ch;
				ch = fgetc(IN);
			}
			//token�ַ�������Ҫ�����ֹ��
			token[i] = '\0';
			//�ж��ǹؼ��ֻ��Ǳ�ʶ��
			isKeyWordOrID(token,idNump);
		} else if(ch>= '0'&&ch<='9') {
			i = 0;
			token[i++] = ch;
			ch = fgetc(IN);
			while(isDigit(ch)) {
				token[i++] = ch;
				ch = fgetc(IN);
			}
			//�����һλΪС����
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
				//������ת����2�����ַ��� 
				itoa(atoi(token),token,2);
			}
			insertDigit(token,digitalLen);
			//ʶ������� 
		} else if(ch == '/'){	//������ע�ͣ�Ҳ�п����ǳ��� 
			token[0] = ch;
			if(!isDividOrAnno(IN,token,&line_Num)){//exception���� 
				fclose(IN);
				return -1;
			}
			ch = fgetc(IN);
		}else{//������� 
			token[0] = ch;
			isOthers(IN,token);
			ch = fgetc(IN);
		}
		//����token 
		memset(token,0,strlen(token));
	}
	fclose(IN);
}
int main() {
	//����
	int lineNum;
	//��ʶ�����е���Ŀ
	int idNum = 0;
	//�������е���Ŀ
	int digitalLen = 0;
	//token�ַ����飬��Ź��ɵ��ʷ��ŵ��ַ���
	char token[MAX_SIZE];
	//�ļ�ָ��
	char fileName[50];
	printf("hello"); 
//	system("pause");
	printf("�����ļ�����\n");
	//strcpy(fileName,"F:\\Quick_Sort.txt"); 
	scanf("%s",fileName);
	fileSC(fileName,token,&idNum,&digitalLen);
	return 0;
}
