#include <iostream>
#include <Vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
//NFA��DFA�����ݽṹNFA/DFA = {Q,��,F,S,Z}
//Q,��ʹ��vector���д洢 
class DFA{
	public:
		vector<int> Q;
		vector<int> Sigma;
		//ʹ��map���洢ӳ���ϵ ,����DFA�е�һ��״̬+����ֻ��ת����һ��״̬����˲���Ҫʹ��Vector 
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
		//ʹ��map���洢ӳ���ϵ,keyֵΪ�������,valueΪ������ת�� 
		map<int, map<int,vector<int> > > function;
		vector<int> S;
		vector<int> Z;
	public:
		//��д�ղι��� 
		NFA();
		vector<int> getEpsilon_Closure(vector<int> S); 
		DFA NFAToDFA();	
		void printNFA();	
};
//��д�ղι��� 
NFA::NFA(){
	fstream fin;
	int tmp;
	string fileName="NFA.txt";
	cout << "�������ļ�����";
//	cin >> fileName; 
	string line;
	stringstream ss; //������
    freopen(fileName.c_str(), "r", stdin);
    getline(cin, line);
    ss << line; //�����д�ֵ
    if (!ss.eof()) {
        while (ss >> tmp) //��ȡint����
        	Q.push_back(tmp); //���浽vector
    }
    ss.clear();
    getline(cin, line);
    ss << line; //�����д�ֵ
    if (!ss.eof()) {
        while (ss >> tmp) //��ȡint����
        	Sigma.push_back(tmp); //���浽vector
    }
    ss.clear();
    int funcSize; 
    getline(cin, line);
	ss << line; //�����д�ֵ
    if (!ss.eof()) {
        while (ss >> tmp) //��ȡint����
        	funcSize = tmp; //���浽vector
    }
	for(int i = 0; i < funcSize; i++){
		vector<int> nodeInfo;
		ss.clear();
		getline(cin, line);
			ss << line; //�����д�ֵ
    	if (!ss.eof()) {
        	while (ss >> tmp) //��ȡint����
        		nodeInfo.push_back(tmp); //���浽vector
    	}
		nodeInfo.push_back(tmp);
		//ͨ�������Ϊ keyֵ 
		function[nodeInfo[0]][nodeInfo[1]] = nodeInfo;
	}
	ss.clear();
    getline(cin, line);
    ss << line; //�����д�ֵ
    if (!ss.eof()) {
        while (ss >> tmp) //��ȡint����
        	S.push_back(tmp); //���浽vector
    }
	ss.clear();
    getline(cin, line);
    ss << line; //�����д�ֵ
    if (!ss.eof()) {
        while (ss >> tmp) //��ȡint����
        	Z.push_back(tmp); //���浽vector
    }
    fclose(stdin);
}
void NFA::printNFA(){
	cout << "NFA(input��" << endl;
	cout << "    ״̬����{";
	for(int i = 0; i < Q.size();i++){
		cout << Q[i]<< " ";
	}
	cout << "}"<<endl;
	cout << "    ���ű�{";
	for(int i = 0; i < Sigma.size();i++){
		cout << Sigma[i]<< " ";
	}
	cout << "}"<<endl;
	cout << "״̬ת����"<<endl;
	map<int, map<int,vector<int> > >::iterator funcIti;
	map<int,vector<int> >::iterator funcItk;
	for(funcIti = function.begin(); funcIti != function.end(); funcIti++){
		for(funcItk = funcIti->second.begin();funcItk != funcIti->second.end(); funcItk++){
			for(int j = 2;j < funcItk->second.size(); j++){
				cout << "   (" << funcItk->second[0] << "," << funcItk->second[1] << ")->" << funcItk->second[j] << endl;
			}
		}
	}
	cout << "��ʼ״̬��{";
	for(int i = 0; i < S.size();i++){
		cout << S[i]<< " ";
	}
	cout << "}"<<endl;
	cout << "����״̬��{";
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
			if(funcIti == function.end()){ //keyֵ������
				continue;
			}else{
				funcItj = funcIti->second.find(-1);
				if(funcItj == funcIti->second.end()){//epsilon������ 
					continue;
				}
				else{
				//����epsilon 
				tmp.insert(tmp.end(),funcItj->second.begin()+2,funcItj->second.end());
				}
			}
		}
		//��SNew���������ȥ�� 
		sort(tmp.begin(),tmp.end());
		tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
	}while(tmp != SNew);
//	for(int i = 0; i < SNew.size(); i++){
//		cout << SNew[i] << " ";
//	}
	return SNew;
}
//NFAתDFA 
DFA NFA::NFAToDFA(){
	DFA dfa;
	dfa.Sigma = Sigma;
	vector<int> SNew = getEpsilon_Closure(S);
	int dfaQ = 0;
	int workingIndex;
	vector<int> J,U;
	vector<vector <int> > QNew;
	//workList�б������QNew��δ�����״̬���±� 
	vector<int> workList;
	QNew.push_back(SNew);workList.push_back(dfaQ);
	dfa.Q.push_back(dfaQ);
	dfa.S.push_back(dfaQ);
	//�ж�S'�Ƿ���Ҫ����nfa.Z 
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
	while(workList.size() != 0){	//��workList�в�Ϊ��ʱ����ѭ��
		workingIndex = workList[workList.size()-1];
		workList.pop_back();
		for(int i = 0; i < Sigma.size(); i++){	//������ĸ���е�ÿ������ 
			J.clear();U.clear();
			//���㼯��J 
			for(int j = 0; j < QNew[workingIndex].size(); j++){	//����δ��ǵ�ÿһ��״̬ 
				funcIti = function.find(QNew[workingIndex][j]);
				if(funcIti == function.end()){ //keyֵ������
					continue;
				}else{
					funcItj = funcIti->second.find(Sigma[i]);
					if(funcItj == funcIti->second.end()){//Sigma[j]������ 
						continue;
					}
					else{
						//����Sigma[j]
						J.insert(J.end(),funcItj->second.begin()+2,funcItj->second.end());
					}
				}
			}
			//��J���������ȥ��
			sort(J.begin(),J.end());
			J.erase(unique(J.begin(),J.end()), J.end());
			//����U��
			U =  getEpsilon_Closure(J);
			bool findInQNew = false;
			findFinal = false;
			if(U.size() != 0){
				for(int k = 0; k < QNew.size(); k++){
					if(U == QNew[k]){	//�ҵ����� 
						findInQNew = true;
						dfa.function[workingIndex][Sigma[i]] = k;
					}
				}
				//δ�ҵ�����
				if(!findInQNew){
					QNew.push_back(U);
					dfaQ++;
					workList.push_back(dfaQ);
					dfa.function[workingIndex][Sigma[i]] = dfaQ;
					dfa.Q.push_back(dfaQ);
					for(int k = 0; k < U.size(); k++){	//�ж��Ƿ���Ҫ�����ռ� 
						if(find(Z.begin(),Z.end(),U[k]) != Z.end()){
							findFinal = true;
							break;
						}
					}
					//��Ҫ����
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
	cout << "DFA(output��" << endl;
	cout << "    ״̬����{";
	for(int i = 0; i < Q.size();i++){
		cout << Q[i]<< " ";
	}
	cout << "}"<<endl;
	cout << "    ���ű�{";
	for(int i = 0; i < Sigma.size();i++){
		cout << Sigma[i]<< " ";
	}
	cout << "}"<<endl;
	cout << "״̬ת����"<<endl;
	map<int, map<int,int > >::iterator funcIti;
	map<int,int >::iterator funcItk;
	for(funcIti = function.begin(); funcIti != function.end(); funcIti++){
		for(funcItk = funcIti->second.begin();funcItk != funcIti->second.end(); funcItk++){
			cout << "   (" << funcIti->first << "," << funcItk->first << ")->" << funcItk->second << endl;
		}
	}
	cout << "��ʼ״̬��{";
	for(int i = 0; i < S.size();i++){
		cout << S[i]<< " ";
	}
	cout << "}"<<endl;
	cout << "����״̬��{";
	for(int i = 0; i < Z.size();i++){
		cout << Z[i]<< " ";
	}
	cout << "}"<<endl;
}
//Ѱ��Image
vector<int> DFA::findImage(vector<int> S,int Input){
	vector<int> image;
	map<int,map<int,int> >::iterator funcIti;
	map<int,int>::iterator funcItj;
	vector<int>::iterator SIt;
	for(int i = 0; i < Q.size(); i++){	//����ÿһ��״̬ 
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
	//��image���������ȥ��
	sort(image.begin(),image.end());
	image.erase(unique(image.begin(),image.end()), image.end());
	return image;
}
//DFA��С��
void DFA::minDFA(){
	DFA mindfa;
	vector<vector <int> > Pi;
	vector<vector <int> > PiNew;
	vector<int> q1;
	vector<int> q2 = Z;
	vector<int> image;
	//�� 
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
	int minQ;//һ���ȼ���Ĵ�����
	map<int, map<int, int> >::iterator funci;
	map<int,int>::iterator funcj; 
	cout << "minDFA:" << endl << "Q:{";
	for(int i = 0; i < Pi.size(); i++){
		if(Pi[i].size() > 1){
			//minQ = Pi[i][0]; //ѡȡ��һ��ֵΪ����ֵ 
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
