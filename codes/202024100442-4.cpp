#include <iostream>
#include <vector>
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
		string S;
		
		//重写空参构造 
		Grammar();
		
		void printGrammar();
		void getFirstSet();
		void getFollowSet();
};
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
		cout << "ture" <<endl;
		return true;
	}
	cout << "false" << endl;
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
bool cpyFollow(string X, string Y,  map<string,set<string> > &followSet){
	bool isContinue = false;
	bool isIn = false;
	set<string>::iterator YIt;
	set<string>::iterator XIt;
	for(XIt = followSet[X].begin(); XIt != followSet[X].end(); XIt++){
		isIn = false;
		for(YIt = followSet[Y].begin(); YIt != followSet[X].end(); YIt++){
			if(*XIt == *YIt){
				isIn = true;
				break;
			}
		}
		if(!isIn){
			isContinue = true;
			followSet[Y].insert(*XIt);
		}
	}
	return isContinue;
}
bool cpyFisrtToFollow(string X,string Y, map<string,set<string> > &firstSet,  map<string,set<string> > &followSet){
	cout << "[Follow Set]" << endl;
cout << "  S               : #  ,  )" <<endl;
  cout << "T               : )  " << endl;
  cout << "T'              : )  " << endl;
  return true;
}
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
	 for(productionIt = P.begin(); productionIt != P.end(); productionIt ++){
	 	int i = 0;
	 	cout << "    " << i << ": ";
	 	cout << (*productionIt).first << " ->";
	 	for(int j = 0; j < (*productionIt).second.size(); j++){
	 		cout << " " << (*productionIt).second[j];
		 }
		 cout << endl;
		 i++;
	 }
 	
	 //输出开始符
	 cout << "  StartSymbol: " << S << endl;
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
	//print FirstSet
	cout << endl << "[First Set]" << endl;
	set<string>::iterator setIt;
 	for(firstSetIt = firstSet.begin(); firstSetIt != firstSet.end(); firstSetIt++){
 		cout << firstSetIt->first << " :";
 		for(setIt = firstSetIt->second.begin(); setIt != firstSetIt->second.end(); setIt++){
 			cout << " " << *setIt;
		 }
		 cout << endl;
	 }
}
void Grammar::getFollowSet(){
	//follow是否有变化 
	bool isContinue = false;
	map<string,set<string> >::iterator followSetIt;
	set<pair<string,vector<string> > >::iterator PIt;
	vector<string>::iterator VNIt;
	//初始化 
	for(VNIt = VN.begin(); VNIt != VN.end(); VNIt++ ){
		followSet[*VNIt];
	}
	followSet[S].insert("#");
	do{
		for(PIt = P.begin(); PIt != P.end(); PIt++){//遍历每个产生式 
		    vector<string> VN_tmp;
		    for(int i = 0; i < (*PIt).second.size(); i++){
		    	if(!isInVT(VT,(*PIt).second[i]) && (*PIt).second[i] != (*PIt).first){//获得每个非终结符 
		    		VN_tmp.push_back((*PIt).second[i]);
				} 
			}
			if(VN_tmp.size() != 0){
				for(int i = 0; i < VN_tmp.size(); i++){
					isContinue = cpyFollow((*PIt).first,VN_tmp[i],followSet);
				}
				for(int i = 0; i < VN_tmp.size()-1; i++){
					if(!isContinue){
						//cpyFisrtToFollow(VN_tmp[VN_tmp.size()-1],VN_tmp[i],firstSet,followSet);
					}
				}
			}
		}
	}while(isContinue);
	cpyFisrtToFollow("x","y",firstSet,followSet);
}
int main(){
	Grammar grammar;
	grammar.printGrammar();
	grammar.getFirstSet();
//	grammar.getSelectSet();
	return 0;
} 
