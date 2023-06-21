#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <string>
#include <fstream>
using namespace std;
class Grammar{
	public:
 	    vector<string> VN;
		vector<string> VT;
		set<pair<string,vector<string> > > P;
		map<string,set<string> > firstSet;
		map<string,set<string> > followSet;
		map<pair<string,vector<string> >,set<string> > selectSet;
		map<pair<string,string>, vector<string> > preAnTable;// 预测分析表
		string S;
		
		//重写空参构造 
		Grammar();
		
		void printGrammar();
		void printSet();
		void getFirstSet();
		void getFollowSet();
		void getSelectSet();
		void makeTable();
		void printTable();
		bool Analyse(string str);
};
/*===================工具函数========================*/ 
bool isInVT(vector<string> &VT,string sign){
	 vector<string>::iterator VTIt;
	 for(VTIt = VT.begin(); VTIt != VT.end(); VTIt++){
	 	if(*VTIt == sign){
	 		return true;
		 }
	 }
	 return false;	
}
bool isInFirstSet(string key, string sign,map<string,set<string> > &firstSet){
	set<string>::iterator firstSetIt;
	firstSetIt = firstSet[key].find(sign);
	if(firstSetIt != firstSet[key].end()){
		return true;
	}
	return false;
}
bool isCopyFirstSet(string X, string Y,map<string,set<string> > &firstSet){
	bool isContinue = false;
	bool isIn = false;
	set<string>::iterator YIt;
	set<string>::iterator XIt;
	for(YIt = firstSet[Y].begin(); YIt != firstSet[Y].end(); YIt++){
		isIn = false;
		if(*YIt == "epsilon"){//如果为epsilon 
			continue;
		}
		for(XIt = firstSet[X].begin(); XIt != firstSet[X].end(); XIt++){
			if(*XIt == *YIt){
				isIn = true;
				break;
			}
		}
		if(!isIn){
			isContinue = true;
			firstSet[X].insert(*YIt);
		}
	}
	return isContinue;
}
//将X的first集copy到y的follow集中 
void cpyFisrtToFollow(string X,string Y, map<string,set<string> > &firstSet,  map<string,set<string> > &followSet){
    set<string>::iterator YIt;
	set<string>::iterator XIt;
	bool isIn = false;
	for(XIt = firstSet[X].begin(); XIt != firstSet[X].end(); XIt++){
		isIn = false;
		if(*XIt == "epsilon"){//如果为epsilon 
			continue;
		}
		for(YIt = followSet[Y].begin(); YIt != followSet[Y].end(); YIt++){
			if(*XIt == *YIt){
				isIn = true;
				break;
			}
		}
		if(!isIn){
			followSet[Y].insert(*XIt);
		}
	} 
    return;
}
string reverseStack(stack<string> s){
	vector<string> stackVector;
	string str;
	while(!s.empty()){
		stackVector.insert(stackVector.begin(),s.top());
		s.pop();
	}
	for(auto it = stackVector.begin(); it != stackVector.end(); it++){
		str.append(*it);
	}
	return str;
}

//空参构造 
Grammar::Grammar(){
	string fileName = "grammar.txt";
	cout << "input file name:";
	cin >> fileName;
	ifstream fin;
	fin.open(fileName.c_str());
	if(!fin.is_open()){
		cout << "file " << fileName << " open failed!"<<endl;
		return;
	}
	int num;
	//input VN
	fin>>num;
	for(int i = 0; i < num; i++){
		string tmp;
		fin >> tmp;
		VN.push_back(tmp);
	}
	//input VT
	fin >> num;
	for(int i = 0; i < num; i++){
		string tmp;
		fin >> tmp;
		VT.push_back(tmp);
	}
	//input P
	fin >> num;
	for(int i = 0; i < num; i++){
	    string tmp;
		string left;
		vector<string> right;
		fin >> left;
		fin >> tmp;
		if(tmp == "->"){
			char scanner;
			tmp="";
			do{
				scanner = fin.get();
				if(scanner != ' ' && scanner != '\n'){
					tmp.append(1,scanner);
				}
				else if(tmp.size() != 0){
					right.push_back(tmp);
					tmp="";
				}
			}while(scanner != '\n');	
		}
		P.insert(make_pair(left,right));
	}
	fin >> S;
	fin.close();
} 
void Grammar::printGrammar(){
	cout << "CFG=(VN,VT,P,S)" << endl;
	//输出非终结符 
	cout << "  VN:";
	vector<string>::iterator vectorIt;
	set<pair<string,vector<string> > >::iterator productionIt;
	for(vectorIt = VN.begin(); vectorIt != VN.end(); vectorIt++){
		cout << " " << *vectorIt;
	}
	cout << endl;
	
	//输出终结符
	cout << "  VT:";
	for(vectorIt = VT.begin(); vectorIt != VT.end(); vectorIt++){
		cout << " " << *vectorIt;
	}
	cout << endl;
	 
	 //输出产生式
	 cout << "  Production:" << endl;
	 int i = 0;
	 for(productionIt = P.begin(); productionIt != P.end(); productionIt ++,i++){
	 	cout << "    " << i << ": ";
	 	cout << (*productionIt).first << " ->";
	 	for(int j = 0; j < (*productionIt).second.size(); j++){
	 		cout << " " << (*productionIt).second[j];
		 }
		 cout << endl;
	 }
 	
	 //输出开始符
	 cout << "  StartSymbol: " << S << endl;
}
void Grammar::printSet(){
	 //print FirstSet
	cout << endl << "[First Set]" << endl;
	map<string,set<string> >::iterator SetIt;
	set<string>::iterator setItemIt;
 	for(SetIt = firstSet.begin(); SetIt != firstSet.end(); SetIt++){
 		cout << SetIt->first << " :";
 		for(setItemIt = SetIt->second.begin(); setItemIt != SetIt->second.end(); setItemIt++){
 			cout << " " << *setItemIt;
		 }
		 cout << endl;
	 }
	 //print FollowSet
	 cout << endl << "[Follow Set]" << endl;
 	 for(SetIt = followSet.begin(); SetIt != followSet.end(); SetIt++){
 	 cout << SetIt->first << " :";
	 for(setItemIt = SetIt->second.begin(); setItemIt != SetIt->second.end(); setItemIt++){
 			cout << " " << *setItemIt;
		 }
		 cout << endl;
	 }
	 //print SelectSet
	 map<pair<string,vector<string> >,set<string> >::iterator selectSetIt;
	 cout << endl << "[Select Set]" << endl;
	 int i = 0;
	 for(selectSetIt = selectSet.begin(); selectSetIt != selectSet.end(); selectSetIt++ ){
	 	cout << "  " << i << ":" << selectSetIt->first.first << " ->";
	 	for(int j = 0; j < selectSetIt->first.second.size(); j++ ){
	 		cout << " " << selectSetIt->first.second[j];
		 }
		 cout << "   :";
 	    for(setItemIt = selectSetIt->second.begin(); setItemIt != selectSetIt->second.end(); setItemIt++){
 	    	cout << " " << *setItemIt;
		 }
		 i++;cout << endl;
	 }
}
void Grammar::getFirstSet(){
	//first是否有变化 
	bool isContinue = false;
	map<string,set<string> >::iterator firstSetIt;
	set<pair<string,vector<string> > >::iterator PIt;
	vector<string>::iterator VNIt;
	//初始化 
	for(VNIt = VN.begin(); VNIt != VN.end(); VNIt++ ){
		firstSet[*VNIt];
	}
	do{
		for(PIt = P.begin(); PIt != P.end(); PIt++){//遍历每个产生式 
		    if((*PIt).second[0] == "epsilon" || isInVT(VT,(*PIt).second[0])){//判断是否为非终结符 
		        if(!isInFirstSet((*PIt).first,(*PIt).second[0],firstSet)){	//非终结符不在firs集中 
		            isContinue = true; 
			    	firstSet[(*PIt).first].insert((*PIt).second[0]);
			    }
		    }else{//对于每个非终结符 
		    	if(firstSet[(*PIt).second[0]].empty()){//如果非终结符对应的first集为空 
		    		continue;//不进行任何操作 
				}else{
					isContinue = isCopyFirstSet((*PIt).first,(*PIt).second[0],firstSet);
				}
			}
		}
	}while(isContinue);
}
void Grammar::getFollowSet(){
	//follow是否有变化 
	int preSize,nowSize;
	preSize = nowSize = 0;
	
	map<string,set<string> >::iterator followSetIt;
	set<pair<string,vector<string> > >::iterator PIt;
	vector<string>::iterator VNIt;
	//初始化 
	for(VNIt = VN.begin(); VNIt != VN.end(); VNIt++ ){
		followSet[*VNIt];
	}
	followSet[S].insert("#");
	do{
		preSize = nowSize;
		//对于每个产生式 
		for(PIt = P.begin();PIt != P.end(); PIt++){
			string left = (*PIt).first;
			vector<string> eachP = (*PIt).second;
			//对与每个产生式 
			for(int i = 0; i < eachP.size(); i++){
				if(eachP[i]== "epsilon" || isInVT(VT,eachP[i])){//若为终结符或epsilon 
					continue; 
				}else if(i == eachP.size() -1){//若满足规则3.1 
                    followSet[eachP[i]].insert(followSet[left].begin(),followSet[left].end());
				}else{
					if(isInVT(VT,eachP[i+1])){//如果后面一个是终结符 
						followSet[eachP[i]].insert(eachP[i+1]);//插入非终结符 
					}else{
						//把first[eachP[i+1]]-epsilon加入
						cpyFisrtToFollow(eachP[i+1],eachP[i],firstSet,followSet);
						if(isInFirstSet(eachP[i+1],"epsilon",firstSet)){
							followSet[eachP[i]].insert(followSet[left].begin(),followSet[left].end());
						}
					}
				}
			}
		}
		nowSize = 0;
		for(int i = 0; i < VN.size(); i++){
			nowSize += followSet[VN[i]].size();
		}
	}while(preSize != nowSize);
}
void Grammar::getSelectSet(){
	set<pair<string,vector<string> > >::iterator PIt;
	for(PIt = P.begin(); PIt != P.end(); PIt++){	//初始化 
		selectSet[*PIt];
	}
	for(PIt = P.begin(); PIt != P.end(); PIt++){	//求解select集 
		string left = PIt->first;
		string eachP = PIt->second[0];
		if(isInVT(VT,eachP)){	//终结符 
			selectSet[*PIt].insert(eachP);
		}else{
			if(eachP == "epsilon" || isInFirstSet(eachP,"epsilon",firstSet)){
				if(eachP != "epsilon"){
					selectSet[*PIt].insert(firstSet[eachP].begin(),firstSet[eachP].end());
					selectSet[*PIt].erase("epsilon");
				}
				//cp follow集 
				selectSet[*PIt].insert(followSet[left].begin(),followSet[left].end());	
			}else{//cp first集 
				selectSet[*PIt].insert(firstSet[eachP].begin(),firstSet[eachP].end()); 
			}
		} 
	}
}
void Grammar::makeTable(){
	cout << "开始构建预测分析表......" << endl;
	for(auto selectIt = selectSet.begin(); selectIt != selectSet.end(); selectIt++){
		string pairLeft = selectIt->first.first;
		vector<string> pairRight = selectIt->first.second;
		for(auto setIt = selectIt->second.begin(); setIt != selectIt->second.end(); setIt++){
			preAnTable.insert(pair<pair<string,string>,vector<string> >(pair<string,string>(pairLeft,*setIt),pairRight));
		}
	}
}
void Grammar::printTable(){ 
	cout << "=================================================" << endl << "                   预测分析表"<< endl << "=================================================" << endl;
	cout << '\t';
	for(auto VTIt = VT.begin(); VTIt != VT.end(); VTIt++){
		cout << *VTIt << '\t';
	}
	cout << "#";
	cout <<endl << "=================================================" << endl;
	for(auto VNIt = VN.begin(); VNIt != VN.end(); VNIt++){
		cout << *VNIt << '\t';
		for(auto VTIt = VT.begin(); VTIt != VT.end(); VTIt++){
			pair<string,string> tmp;
			tmp= make_pair(*VNIt,*VTIt);
		 	auto tableIt = preAnTable.find(tmp);
		 	if(tableIt == preAnTable.end()){
		 		cout << "-1";
			 }
			else{
				for(int i = 0; i < tableIt->second.size(); i++){
					cout << tableIt->second[i];
				}
			}
			cout << '\t';
		}
		pair<string,string> tmp;
		tmp= make_pair(*VNIt,"#");
 		auto tableIt = preAnTable.find(tmp);
	 	if(tableIt == preAnTable.end()){
		 	cout << "-1";
		 }
		else{
			for(int i = 0; i < tableIt->second.size(); i++){
				cout << tableIt->second[i];
			}
		}
		cout << endl;
	}
	cout << "=================================================" << endl;
}
bool Grammar::Analyse(string str){
	 cout << "开始分析……" << endl;
	 cout << "步骤\t\t符号栈\t\t输入串\t\t所用产生式\t\t状态"<<endl<<"============================================================================"<<endl;
	 //步骤数 
	 int step = 1;
	 //分析栈
	 stack<string> stackA;
	 
	 string strRemain = str;
	 string state = "待分析" ;
	 string production = "\t";
	 //开始符号入栈 
	 stackA.push("#");stackA.push(S);
	 cout << step << "\t\t" << reverseStack(stackA)<<"\t\t" << strRemain <<"\t\t" << production<<"\t\t"<<state << endl;
	 for(auto strIt = str.begin();strIt != str.end();){
	 	string s="";
	 	state = "待分析" ;
	 	production = "";
	 	s.push_back(*strIt);
	 	//当前栈顶元素 
	 	string top = stackA.top();
	 	if(isInVT(VT,top)){//如果是终结符 
	 		if(top == s){//栈顶元素与输入字符匹配 
	 			stackA.pop();
	 			//删除字符串的指定字符 
				strRemain.erase(0,1);
	 			state = "“" + top + "”匹配";
	 			strIt++;
			 }else{
				cout << "分析出错:" <<top<<"和" <<s<< "不匹配" << endl;
				return false;
			 } 
 		}else if(top == "#"){
 			if(s == "#"){
 				cout << "分析结束" << endl;
 				cout << "============================================================================"<<endl;
 				return true; 
			 }else{
			 	cout << "分析失败" << endl;
			 	cout << "============================================================================"<<endl;
			 	return false;
			 }
		 }else{//top为非终结符 
		 	if(preAnTable.find(pair<string,string>(top,s)) == preAnTable.end()){
		 	    cout << "分析出错，不能找到表达式" <<endl;
			 	return false;
	 		}else{//分析表中能找到 
	 			vector<string> pushVec = preAnTable.find(pair<string,string>(top,s))->second;
	 			string pushStr = "";
	 			for(int i = 0; i < pushVec.size(); i++){
	 				pushStr += pushVec[i];
				 }
	 			production = top + " -> "+pushStr;
	 			//反向进栈！ 
	 			stackA.pop();
	 			if(pushStr != "epsilon"){
 				    for(int i = pushVec.size()-1; i >= 0; i--){
	 				    stackA.push(pushVec[i]);
				    }
				 }
			 }
		 }
		 step++;
 		 cout << step << "\t\t" << reverseStack(stackA)<<"\t\t" << strRemain <<"\t\t" << production<<"\t\t"<<state << endl;
	 }
}
int main(){
	Grammar grammar;
	grammar.printGrammar();
	grammar.getFirstSet();
	grammar.getFollowSet();
	grammar.getSelectSet();
	grammar.printSet();
	grammar.makeTable();
	grammar.printTable();
	cout << "分析表已构造完成......" <<endl;
	string strFileName = "str.txt";
	cout << "输入符号串所在的文件名:" ;
	cin >> strFileName;
	ifstream fin;
	fin.open(strFileName.c_str());
	if(!fin.is_open()){
		cout << "file:" << strFileName << " open failed!"<<endl;
		return -1;
	}
	string str;
	fin >> str;
	bool isSuccess = false;
	if(str.size() == 0){
		cout << "file:" << strFileName << "为空！请检查后重新输入！"<<endl;
		return -1; 
	}else{
		//开始分析
		str.append("#");
		isSuccess = grammar.Analyse(str); 
		if(isSuccess){
			cout << "符号串"+str+"是该文法正确句子" <<endl;
		}else{
			cout << "符号串"+str+"不是该文法正确句子" <<endl;
		}
	}
	return 0;
} 
