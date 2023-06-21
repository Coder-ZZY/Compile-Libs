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
		map<pair<string,string>, vector<string> > preAnTable;// Ԥ�������
		string S;
		
		//��д�ղι��� 
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
/*===================���ߺ���========================*/ 
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
		if(*YIt == "epsilon"){//���Ϊepsilon 
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
//��X��first��copy��y��follow���� 
void cpyFisrtToFollow(string X,string Y, map<string,set<string> > &firstSet,  map<string,set<string> > &followSet){
    set<string>::iterator YIt;
	set<string>::iterator XIt;
	bool isIn = false;
	for(XIt = firstSet[X].begin(); XIt != firstSet[X].end(); XIt++){
		isIn = false;
		if(*XIt == "epsilon"){//���Ϊepsilon 
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

//�ղι��� 
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
	//������ս�� 
	cout << "  VN:";
	vector<string>::iterator vectorIt;
	set<pair<string,vector<string> > >::iterator productionIt;
	for(vectorIt = VN.begin(); vectorIt != VN.end(); vectorIt++){
		cout << " " << *vectorIt;
	}
	cout << endl;
	
	//����ս��
	cout << "  VT:";
	for(vectorIt = VT.begin(); vectorIt != VT.end(); vectorIt++){
		cout << " " << *vectorIt;
	}
	cout << endl;
	 
	 //�������ʽ
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
 	
	 //�����ʼ��
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
	//first�Ƿ��б仯 
	bool isContinue = false;
	map<string,set<string> >::iterator firstSetIt;
	set<pair<string,vector<string> > >::iterator PIt;
	vector<string>::iterator VNIt;
	//��ʼ�� 
	for(VNIt = VN.begin(); VNIt != VN.end(); VNIt++ ){
		firstSet[*VNIt];
	}
	do{
		for(PIt = P.begin(); PIt != P.end(); PIt++){//����ÿ������ʽ 
		    if((*PIt).second[0] == "epsilon" || isInVT(VT,(*PIt).second[0])){//�ж��Ƿ�Ϊ���ս�� 
		        if(!isInFirstSet((*PIt).first,(*PIt).second[0],firstSet)){	//���ս������firs���� 
		            isContinue = true; 
			    	firstSet[(*PIt).first].insert((*PIt).second[0]);
			    }
		    }else{//����ÿ�����ս�� 
		    	if(firstSet[(*PIt).second[0]].empty()){//������ս����Ӧ��first��Ϊ�� 
		    		continue;//�������κβ��� 
				}else{
					isContinue = isCopyFirstSet((*PIt).first,(*PIt).second[0],firstSet);
				}
			}
		}
	}while(isContinue);
}
void Grammar::getFollowSet(){
	//follow�Ƿ��б仯 
	int preSize,nowSize;
	preSize = nowSize = 0;
	
	map<string,set<string> >::iterator followSetIt;
	set<pair<string,vector<string> > >::iterator PIt;
	vector<string>::iterator VNIt;
	//��ʼ�� 
	for(VNIt = VN.begin(); VNIt != VN.end(); VNIt++ ){
		followSet[*VNIt];
	}
	followSet[S].insert("#");
	do{
		preSize = nowSize;
		//����ÿ������ʽ 
		for(PIt = P.begin();PIt != P.end(); PIt++){
			string left = (*PIt).first;
			vector<string> eachP = (*PIt).second;
			//����ÿ������ʽ 
			for(int i = 0; i < eachP.size(); i++){
				if(eachP[i]== "epsilon" || isInVT(VT,eachP[i])){//��Ϊ�ս����epsilon 
					continue; 
				}else if(i == eachP.size() -1){//���������3.1 
                    followSet[eachP[i]].insert(followSet[left].begin(),followSet[left].end());
				}else{
					if(isInVT(VT,eachP[i+1])){//�������һ�����ս�� 
						followSet[eachP[i]].insert(eachP[i+1]);//������ս�� 
					}else{
						//��first[eachP[i+1]]-epsilon����
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
	for(PIt = P.begin(); PIt != P.end(); PIt++){	//��ʼ�� 
		selectSet[*PIt];
	}
	for(PIt = P.begin(); PIt != P.end(); PIt++){	//���select�� 
		string left = PIt->first;
		string eachP = PIt->second[0];
		if(isInVT(VT,eachP)){	//�ս�� 
			selectSet[*PIt].insert(eachP);
		}else{
			if(eachP == "epsilon" || isInFirstSet(eachP,"epsilon",firstSet)){
				if(eachP != "epsilon"){
					selectSet[*PIt].insert(firstSet[eachP].begin(),firstSet[eachP].end());
					selectSet[*PIt].erase("epsilon");
				}
				//cp follow�� 
				selectSet[*PIt].insert(followSet[left].begin(),followSet[left].end());	
			}else{//cp first�� 
				selectSet[*PIt].insert(firstSet[eachP].begin(),firstSet[eachP].end()); 
			}
		} 
	}
}
void Grammar::makeTable(){
	cout << "��ʼ����Ԥ�������......" << endl;
	for(auto selectIt = selectSet.begin(); selectIt != selectSet.end(); selectIt++){
		string pairLeft = selectIt->first.first;
		vector<string> pairRight = selectIt->first.second;
		for(auto setIt = selectIt->second.begin(); setIt != selectIt->second.end(); setIt++){
			preAnTable.insert(pair<pair<string,string>,vector<string> >(pair<string,string>(pairLeft,*setIt),pairRight));
		}
	}
}
void Grammar::printTable(){ 
	cout << "=================================================" << endl << "                   Ԥ�������"<< endl << "=================================================" << endl;
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
	 cout << "��ʼ��������" << endl;
	 cout << "����\t\t����ջ\t\t���봮\t\t���ò���ʽ\t\t״̬"<<endl<<"============================================================================"<<endl;
	 //������ 
	 int step = 1;
	 //����ջ
	 stack<string> stackA;
	 
	 string strRemain = str;
	 string state = "������" ;
	 string production = "\t";
	 //��ʼ������ջ 
	 stackA.push("#");stackA.push(S);
	 cout << step << "\t\t" << reverseStack(stackA)<<"\t\t" << strRemain <<"\t\t" << production<<"\t\t"<<state << endl;
	 for(auto strIt = str.begin();strIt != str.end();){
	 	string s="";
	 	state = "������" ;
	 	production = "";
	 	s.push_back(*strIt);
	 	//��ǰջ��Ԫ�� 
	 	string top = stackA.top();
	 	if(isInVT(VT,top)){//������ս�� 
	 		if(top == s){//ջ��Ԫ���������ַ�ƥ�� 
	 			stackA.pop();
	 			//ɾ���ַ�����ָ���ַ� 
				strRemain.erase(0,1);
	 			state = "��" + top + "��ƥ��";
	 			strIt++;
			 }else{
				cout << "��������:" <<top<<"��" <<s<< "��ƥ��" << endl;
				return false;
			 } 
 		}else if(top == "#"){
 			if(s == "#"){
 				cout << "��������" << endl;
 				cout << "============================================================================"<<endl;
 				return true; 
			 }else{
			 	cout << "����ʧ��" << endl;
			 	cout << "============================================================================"<<endl;
			 	return false;
			 }
		 }else{//topΪ���ս�� 
		 	if(preAnTable.find(pair<string,string>(top,s)) == preAnTable.end()){
		 	    cout << "�������������ҵ����ʽ" <<endl;
			 	return false;
	 		}else{//�����������ҵ� 
	 			vector<string> pushVec = preAnTable.find(pair<string,string>(top,s))->second;
	 			string pushStr = "";
	 			for(int i = 0; i < pushVec.size(); i++){
	 				pushStr += pushVec[i];
				 }
	 			production = top + " -> "+pushStr;
	 			//�����ջ�� 
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
	cout << "�������ѹ������......" <<endl;
	string strFileName = "str.txt";
	cout << "������Ŵ����ڵ��ļ���:" ;
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
		cout << "file:" << strFileName << "Ϊ�գ�������������룡"<<endl;
		return -1; 
	}else{
		//��ʼ����
		str.append("#");
		isSuccess = grammar.Analyse(str); 
		if(isSuccess){
			cout << "���Ŵ�"+str+"�Ǹ��ķ���ȷ����" <<endl;
		}else{
			cout << "���Ŵ�"+str+"���Ǹ��ķ���ȷ����" <<endl;
		}
	}
	return 0;
} 
