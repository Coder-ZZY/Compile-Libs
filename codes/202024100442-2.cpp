#include <iostream>
#include <Vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
//NFA与DFA的数据结构NFA/DFA = {Q,Σ,F,S,Z}
//Q,Σ使用vector进行存储 
class DFA{
	public:
		vector<int> Q;
		vector<int> Sigma;
		//使用map来存储映射关系 ,由于DFA中的一个状态+输入只能转入另一种状态，因此不需要使用Vector 
		map<int,map<int,int > >function;
		vector<int> S;
		vector<int> Z;
		vector<int> findImage(vector<int> S,int Input);
		void minDFA();
		void printDFA();	
};
class NFA{
	private: 
		vector<int> Q;
		vector<int> Sigma;
		//使用map来存储映射关系,key值为结点名称,value为输入与转换 
		map<int, map<int,vector<int> > > function;
		vector<int> S;
		vector<int> Z;
	public:
		//重写空参构造 
		NFA();
		vector<int> getEpsilon_Closure(vector<int> S); 
		DFA NFAToDFA();	
		void printNFA();	
};
//重写空参构造 
NFA::NFA(){
	fstream fin;
	int tmp;
	string fileName="NFA.txt";
	cout << "请输入文件名：";
//	cin >> fileName; 
	string line;
	stringstream ss; //输入流
    freopen(fileName.c_str(), "r", stdin);
    getline(cin, line);
    ss << line; //向流中传值
    if (!ss.eof()) {
        while (ss >> tmp) //提取int数据
        	Q.push_back(tmp); //保存到vector
    }
    ss.clear();
    getline(cin, line);
    ss << line; //向流中传值
    if (!ss.eof()) {
        while (ss >> tmp) //提取int数据
        	Sigma.push_back(tmp); //保存到vector
    }
    ss.clear();
    int funcSize; 
    getline(cin, line);
	ss << line; //向流中传值
    if (!ss.eof()) {
        while (ss >> tmp) //提取int数据
        	funcSize = tmp; //保存到vector
    }
	for(int i = 0; i < funcSize; i++){
		vector<int> nodeInfo;
		ss.clear();
		getline(cin, line);
			ss << line; //向流中传值
    	if (!ss.eof()) {
        	while (ss >> tmp) //提取int数据
        		nodeInfo.push_back(tmp); //保存到vector
    	}
		nodeInfo.push_back(tmp);
		//通过结点作为 key值 
		function[nodeInfo[0]][nodeInfo[1]] = nodeInfo;
	}
	ss.clear();
    getline(cin, line);
    ss << line; //向流中传值
    if (!ss.eof()) {
        while (ss >> tmp) //提取int数据
        	S.push_back(tmp); //保存到vector
    }
	ss.clear();
    getline(cin, line);
    ss << line; //向流中传值
    if (!ss.eof()) {
        while (ss >> tmp) //提取int数据
        	Z.push_back(tmp); //保存到vector
    }
    fclose(stdin);
}
void NFA::printNFA(){
	cout << "NFA(input）" << endl;
	cout << "    状态集：{";
	for(int i = 0; i < Q.size();i++){
		cout << Q[i]<< " ";
	}
	cout << "}"<<endl;
	cout << "    符号表：{";
	for(int i = 0; i < Sigma.size();i++){
		cout << Sigma[i]<< " ";
	}
	cout << "}"<<endl;
	cout << "状态转换："<<endl;
	map<int, map<int,vector<int> > >::iterator funcIti;
	map<int,vector<int> >::iterator funcItk;
	for(funcIti = function.begin(); funcIti != function.end(); funcIti++){
		for(funcItk = funcIti->second.begin();funcItk != funcIti->second.end(); funcItk++){
			for(int j = 2;j < funcItk->second.size(); j++){
				cout << "   (" << funcItk->second[0] << "," << funcItk->second[1] << ")->" << funcItk->second[j] << endl;
			}
		}
	}
	cout << "开始状态：{";
	for(int i = 0; i < S.size();i++){
		cout << S[i]<< " ";
	}
	cout << "}"<<endl;
	cout << "结束状态：{";
	for(int i = 0; i < Z.size();i++){
		cout << Z[i]<< " ";
	}
	cout << "}"<<endl;
}
//getEpsilon_Closure
vector<int> NFA::getEpsilon_Closure(vector<int> S){
	map<int, map<int,vector<int> > >::iterator funcIti;
	map<int, vector<int> >::iterator funcItj;
	//S'
	vector<int> SNew;
	for(int i = 0; i < S.size(); i++){
		SNew.push_back(S[i]);
	}
	vector<int> tmp = SNew;
	do{
		SNew.assign(tmp.begin(),tmp.end());
		for(int i = 0; i < tmp.size(); i++){
			funcIti = function.find(tmp[i]);
			if(funcIti == function.end()){ //key值不存在
				continue;
			}else{
				funcItj = funcIti->second.find(-1);
				if(funcItj == funcIti->second.end()){//epsilon不存在 
					continue;
				}
				else{
				//存在epsilon 
				tmp.insert(tmp.end(),funcItj->second.begin()+2,funcItj->second.end());
				}
			}
		}
		//对SNew进行排序和去重 
		sort(tmp.begin(),tmp.end());
		tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
	}while(tmp != SNew);
//	for(int i = 0; i < SNew.size(); i++){
//		cout << SNew[i] << " ";
//	}
	return SNew;
}
//NFA转DFA 
DFA NFA::NFAToDFA(){
	DFA dfa;
	dfa.Sigma = Sigma;
	vector<int> SNew = getEpsilon_Closure(S);
	int dfaQ = 0;
	int workingIndex;
	vector<int> J,U;
	vector<vector <int> > QNew;
	//workList中保存的是QNew中未被标记状态的下标 
	vector<int> workList;
	QNew.push_back(SNew);workList.push_back(dfaQ);
	dfa.Q.push_back(dfaQ);
	dfa.S.push_back(dfaQ);
	//判断S'是否需要加入nfa.Z 
	bool findFinal = false;
	for(int i = 0; i < SNew.size(); i++){
		if(find(Z.begin(),Z.end(),SNew[i]) != Z.end()){
			findFinal = true;
			break;
		}
	}
	if(findFinal){
		dfa.Z.push_back(dfaQ);
	}
	map<int, map<int,vector<int> > >::iterator funcIti;
	map<int,vector<int> > ::iterator funcItj;
	while(workList.size() != 0){	//当workList中不为空时进入循环
		workingIndex = workList[workList.size()-1];
		workList.pop_back();
		for(int i = 0; i < Sigma.size(); i++){	//检索字母表中的每个输入 
			J.clear();U.clear();
			//计算集合J 
			for(int j = 0; j < QNew[workingIndex].size(); j++){	//对于未标记的每一个状态 
				funcIti = function.find(QNew[workingIndex][j]);
				if(funcIti == function.end()){ //key值不存在
					continue;
				}else{
					funcItj = funcIti->second.find(Sigma[i]);
					if(funcItj == funcIti->second.end()){//Sigma[j]不存在 
						continue;
					}
					else{
						//存在Sigma[j]
						J.insert(J.end(),funcItj->second.begin()+2,funcItj->second.end());
					}
				}
			}
			//对J进行排序和去重
			sort(J.begin(),J.end());
			J.erase(unique(J.begin(),J.end()), J.end());
			//计算U。
			U =  getEpsilon_Closure(J);
			bool findInQNew = false;
			findFinal = false;
			if(U.size() != 0){
				for(int k = 0; k < QNew.size(); k++){
					if(U == QNew[k]){	//找到集合 
						findInQNew = true;
						dfa.function[workingIndex][Sigma[i]] = k;
					}
				}
				//未找到集合
				if(!findInQNew){
					QNew.push_back(U);
					dfaQ++;
					workList.push_back(dfaQ);
					dfa.function[workingIndex][Sigma[i]] = dfaQ;
					dfa.Q.push_back(dfaQ);
					for(int k = 0; k < U.size(); k++){	//判断是否需要加入终集 
						if(find(Z.begin(),Z.end(),U[k]) != Z.end()){
							findFinal = true;
							break;
						}
					}
					//需要加入
					if(findFinal){
						dfa.Z.push_back(dfaQ);
					}					
				}
				
			}
		}
	}
	return dfa;
}
void DFA::printDFA(){
	cout << "DFA(output）" << endl;
	cout << "    状态集：{";
	for(int i = 0; i < Q.size();i++){
		cout << Q[i]<< " ";
	}
	cout << "}"<<endl;
	cout << "    符号表：{";
	for(int i = 0; i < Sigma.size();i++){
		cout << Sigma[i]<< " ";
	}
	cout << "}"<<endl;
	cout << "状态转换："<<endl;
	map<int, map<int,int > >::iterator funcIti;
	map<int,int >::iterator funcItk;
	for(funcIti = function.begin(); funcIti != function.end(); funcIti++){
		for(funcItk = funcIti->second.begin();funcItk != funcIti->second.end(); funcItk++){
			cout << "   (" << funcIti->first << "," << funcItk->first << ")->" << funcItk->second << endl;
		}
	}
	cout << "开始状态：{";
	for(int i = 0; i < S.size();i++){
		cout << S[i]<< " ";
	}
	cout << "}"<<endl;
	cout << "结束状态：{";
	for(int i = 0; i < Z.size();i++){
		cout << Z[i]<< " ";
	}
	cout << "}"<<endl;
}
//寻找Image
vector<int> DFA::findImage(vector<int> S,int Input){
	vector<int> image;
	map<int,map<int,int> >::iterator funcIti;
	map<int,int>::iterator funcItj;
	vector<int>::iterator SIt;
	for(int i = 0; i < Q.size(); i++){	//对于每一个状态 
		funcIti = function.find(Q[i]);
		if(funcIti == function.end()){
			continue;
		}
		else{
			funcItj = funcIti->second.find(Input);
			if(funcItj == funcIti->second.end()){
				continue;
			}
			else{
				SIt = find(S.begin(),S.end(),Input);
				if(SIt != S.end()){
					image.push_back(Q[i]);
				}
			}
		}
	}
	//对image进行排序和去重
	sort(image.begin(),image.end());
	image.erase(unique(image.begin(),image.end()), image.end());
	return image;
}
//DFA最小化
void DFA::minDFA(){
	DFA mindfa;
	vector<vector <int> > Pi;
	vector<vector <int> > PiNew;
	vector<int> q1;
	vector<int> q2 = Z;
	vector<int> image;
	//求差集 
	set_difference(Q.begin(),Q.end(),Z.begin(),Z.end(),back_inserter(q1));
	Pi.push_back(q1);Pi.push_back(q2);
	bool beenPart;
	do{
		beenPart = false;PiNew.clear();
		for(int i = 0; i < Pi.size(); i++){
			if(Pi[i].size() == 1){
				PiNew.push_back(Pi[i]);
				continue;
			}
			for(int j = 0; j < Sigma.size(); j++){
				image.clear();q1.clear();q2.clear();
				image = findImage(Pi[i],Sigma[j]);
				set_intersection(Pi[i].begin(),Pi[i].end(),image.begin(),image.end(),back_inserter(q1));
				if(q2.size() != 0){
					PiNew.push_back(q1);PiNew.push_back(q2);beenPart = true;cout<< beenPart;break;
				}
			}
			if(beenPart){
				i++;
				while(i < Pi.size()){
					PiNew.push_back(Pi[i]);
					i++;
				}
				break;
			}
		}
		if(beenPart){
			Pi = PiNew;
		}
	}while(beenPart);
	int minQ;//一个等价类的代表结点
	map<int, map<int, int> >::iterator funci;
	map<int,int>::iterator funcj; 
	cout << "minDFA:" << endl << "Q:{";
	for(int i = 0; i < Pi.size(); i++){
		if(Pi[i].size() > 1){
			//minQ = Pi[i][0]; //选取第一个值为代表值 
			//mindfa.Q.push_back(minQ);
			cout << "{";
			for(int j = 0; j < Pi[i].size(); j++){
				cout << Pi[i][j] << " ";
			}
			cout << "}";
		}
		else cout << ",{" << Pi[i][0] << "}";
	}
	cout << "}";
} 
int main(){
	NFA nfa;
	nfa.printNFA();
	DFA dfa = nfa.NFAToDFA();
	dfa.printDFA();
	dfa.minDFA();
	return 0; 
}
