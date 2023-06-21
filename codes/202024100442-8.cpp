#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
/**
 * coded by zzy
 * time 2023/4/26
 */
using namespace std;
/*===================工具函数========================*/
bool isInSet(set<pair<string, vector<string>>> &set, pair<string, vector<string>> &item)
{
    bool isIn = false;
    for (auto setItem : set)
    {
        isIn = false;
        if (setItem.first == item.first)
        {
            for (int i = 0; i < setItem.second.size(); i++)
            {
                if (item.second[i] == setItem.second[i])
                {
                    isIn = true;
                }
                else
                {
                    isIn = false;
                    break;
                }
            }
            if (isIn)
                return true;
        }
    }
    return false;
}
int isInProSet(vector<set<pair<string, vector<string>>>> &projectSet, set<pair<string, vector<string>>> &tmp)
{
    int flag = -1;
    bool index;
    for (auto proItem = projectSet.begin(); proItem != projectSet.end(); proItem++)
    {
        if ((*proItem).size() == tmp.size())
        {
            for (auto setItem : tmp)
            {
                if (!isInSet(*proItem, setItem))
                {
                    index = false;
                    break;
                }
                else
                {
                    index = true;
                }
            }
            if (index)
            {
                flag = proItem - projectSet.begin();
                return flag;
            }
        }
    }
    return -1;
}
bool isInVT(vector<string> &VT, string sign)
{
    vector<string>::iterator VTIt;
    for (VTIt = VT.begin(); VTIt != VT.end(); VTIt++)
    {
        if (*VTIt == sign)
        {
            return true;
        }
    }
    return false;
}
bool isInZ(map<int, map<int, vector<int>>> &function, int sign)
{
    for (auto mapItem : function)
    {
        if (mapItem.first == sign)
        {
            return true;
        }
    }
    return false;
}
//方法重载
bool isInZ(vector<int> &Z, int sign)
{
    for (auto ZItem : Z)
    {
        if (ZItem == sign)
        {
            return true;
        }
    }
    return false;
}
int findIngrammar(set<pair<string, vector<string>>> P, pair<string, vector<string>> proSetItem)
{
    proSetItem.second.erase(find(proSetItem.second.begin(), proSetItem.second.end(), "."));
    bool isIn = false;
    int i = 0;
    for (auto PIt = P.begin(); PIt != P.end(); PIt++)
    {
        isIn = false;
        if ((*PIt).first == proSetItem.first)
        {
            for (int i = 0; i < (*PIt).second.size(); i++)
            {
                if (proSetItem.second[i] != (*PIt).second[i])
                {
                    isIn = false;
                    break;
                }
                else
                {
                    isIn = true;
                }
            }
            if (isIn)
            {
                return i;
            }
        }
        i++;
    }
    return 0;
}
// LR1分析表
class LR1AnaTable
{
public:
    map<int, map<int, string>> Action;
    map<int, map<int, string>> Go;
};
// NFA与DFA的数据结构NFA/DFA = {Q,Σ,F,S,Z}
// Q,Σ使用vector进行存储
class DFA
{
public:
    vector<int> Q;
    vector<int> Sigma;
    // 使用map来存储映射关系 ,由于DFA中的一个状态+输入只能转入另一种状态，因此不需要使用Vector
    map<int, map<int, int>> function;
    vector<int> S;
    vector<int> Z;
    vector<int> findImage(vector<int> S, int Input);
    void minDFA();
    void printDFA();
};
class NFA
{
public:
    vector<int> Q;
    vector<int> Sigma;
    // 使用map来存储映射关系,key值为结点名称,value为输入与转换
    map<int, map<int, vector<int>>> function;
    vector<int> S;
    vector<int> Z;
    vector<int> getEpsilon_Closure(vector<int> S);
    DFA NFAToDFA();
    void printNFA();
};
// getEpsilon_Closure
vector<int> NFA::getEpsilon_Closure(vector<int> S)
{
    map<int, map<int, vector<int>>>::iterator funcIti;
    map<int, vector<int>>::iterator funcItj;
    // S'
    vector<int> SNew;
    for (int i = 0; i < S.size(); i++)
    {
        SNew.push_back(S[i]);
    }
    vector<int> tmp = SNew;
    do
    {
        SNew.assign(tmp.begin(), tmp.end());
        for (int i = 0; i < tmp.size(); i++)
        {
            funcIti = function.find(tmp[i]);
            if (funcIti == function.end())
            { // key值不存在
                continue;
            }
            else
            {
                funcItj = funcIti->second.find(-1);
                if (funcItj == funcIti->second.end())
                { // epsilon不存在
                    continue;
                }
                else
                {
                    // 存在epsilon
                    tmp.insert(tmp.end(), funcItj->second.begin(), funcItj->second.end());
                }
            }
        }
        // 对SNew进行排序和去重
        sort(tmp.begin(), tmp.end());
        tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
    } while (tmp != SNew);
    //	for(int i = 0; i < SNew.size(); i++){
    //		cout << SNew[i] << " ";
    //	}
    return SNew;
}
void NFA::printNFA()
{
    cout << "NFA(input)" << endl;
    cout << "    状态集：{";
    for (int i = 0; i < Q.size(); i++)
    {
        cout << Q[i] << " ";
    }
    cout << "}" << endl;
    cout << "    符号表：{";
    for (int i = 0; i < Sigma.size(); i++)
    {
        cout << Sigma[i] << " ";
    }
    cout << "}" << endl;
    cout << "状态转换：" << endl;
    for (auto mapItem : function)
    {
        for (auto mapItem2 : mapItem.second)
        {
            cout << "  "
                 << "(" << mapItem.first << "," << mapItem2.first << ") -> ";
            for (int i = 0; i < mapItem2.second.size(); i++)
            {
                cout << mapItem2.second[i] << " ";
            }
            cout << endl;
        }
    }
    cout << "开始状态：{";
    for (int i = 0; i < S.size(); i++)
    {
        cout << S[i] << " ";
    }
    cout << "}" << endl;
    cout << "结束状态：{";
    for (int i = 0; i < Z.size(); i++)
    {
        cout << Z[i] << " ";
    }
    cout << "}" << endl;
}
// NFA转DFA
DFA NFA::NFAToDFA()
{
    DFA dfa;
    dfa.Sigma = Sigma;
    vector<int> SNew = getEpsilon_Closure(S);
    int dfaQ = 0;
    int workingIndex;
    vector<int> J, U;
    vector<vector<int>> QNew;
    // workList中保存的是QNew中未被标记状态的下标
    vector<int> workList;
    QNew.push_back(SNew);
    workList.push_back(dfaQ);
    dfa.Q.push_back(dfaQ);
    dfa.S.push_back(dfaQ);
    // 判断S'是否需要加入nfa.Z
    bool findFinal = false;
    for (int i = 0; i < SNew.size(); i++)
    {
        if (find(Z.begin(), Z.end(), SNew[i]) != Z.end())
        {
            findFinal = true;
            break;
        }
    }
    if (findFinal)
    {
        dfa.Z.push_back(dfaQ);
    }
    map<int, map<int, vector<int>>>::iterator funcIti;
    map<int, vector<int>>::iterator funcItj;
    while (workList.size() != 0)
    { // 当workList中不为空时进入循环
        workingIndex = workList[workList.size() - 1];
        workList.pop_back();
        for (int i = 0; i < Sigma.size(); i++)
        { // 检索字母表中的每个输入
            J.clear();
            U.clear();
            // 计算集合J
            for (int j = 0; j < QNew[workingIndex].size(); j++)
            { // 对于未标记的每一个状态
                funcIti = function.find(QNew[workingIndex][j]);
                if (funcIti == function.end())
                { // key值不存在
                    continue;
                }
                else
                {
                    funcItj = funcIti->second.find(Sigma[i]);
                    if (funcItj == funcIti->second.end())
                    { // Sigma[j]不存在
                        continue;
                    }
                    else
                    {
                        // 存在Sigma[j]
                        J.insert(J.end(), funcItj->second.begin(), funcItj->second.end());
                    }
                }
            }
            // 对J进行排序和去重
            sort(J.begin(), J.end());
            J.erase(unique(J.begin(), J.end()), J.end());
            // 计算U。
            U = getEpsilon_Closure(J);
            bool findInQNew = false;
            findFinal = false;
            if (U.size() != 0)
            {
                for (int k = 0; k < QNew.size(); k++)
                {
                    if (U == QNew[k])
                    { // 找到集合
                        findInQNew = true;
                        dfa.function[workingIndex][Sigma[i]] = k;
                    }
                }
                // 未找到集合
                if (!findInQNew)
                {
                    QNew.push_back(U);
                    dfaQ++;
                    workList.push_back(dfaQ);
                    dfa.function[workingIndex][Sigma[i]] = dfaQ;
                    dfa.Q.push_back(dfaQ);
                    for (int k = 0; k < U.size(); k++)
                    { // 判断是否需要加入终集
                        if (find(Z.begin(), Z.end(), U[k]) != Z.end())
                        {
                            findFinal = true;
                            break;
                        }
                    }
                    // 需要加入
                    if (findFinal)
                    {
                        dfa.Z.push_back(dfaQ);
                    }
                }
            }
        }
    }
    return dfa;
}
void DFA::printDFA()
{
    cout << "DFA(output）" << endl;
    cout << "    状态集：{";
    for (int i = 0; i < Q.size(); i++)
    {
        cout << Q[i] << " ";
    }
    cout << "}" << endl;
    cout << "    符号表：{";
    for (int i = 0; i < Sigma.size(); i++)
    {
        cout << Sigma[i] << " ";
    }
    cout << "}" << endl;
    cout << "状态转换：" << endl;
    map<int, map<int, int>>::iterator funcIti;
    map<int, int>::iterator funcItk;
    for (funcIti = function.begin(); funcIti != function.end(); funcIti++)
    {
        for (funcItk = funcIti->second.begin(); funcItk != funcIti->second.end(); funcItk++)
        {
            cout << "   (" << funcIti->first << "," << funcItk->first << ")->" << funcItk->second << endl;
        }
    }
    cout << "开始状态：{";
    for (int i = 0; i < S.size(); i++)
    {
        cout << S[i] << " ";
    }
    cout << "}" << endl;
    cout << "结束状态：{";
    for (int i = 0; i < Z.size(); i++)
    {
        cout << Z[i] << " ";
    }
    cout << "}" << endl;
}
// 寻找Image
vector<int> DFA::findImage(vector<int> S, int Input)
{
    vector<int> image;
    map<int, map<int, int>>::iterator funcIti;
    map<int, int>::iterator funcItj;
    vector<int>::iterator SIt;
    for (int i = 0; i < Q.size(); i++)
    { // 对于每一个状态
        funcIti = function.find(Q[i]);
        if (funcIti == function.end())
        {
            continue;
        }
        else
        {
            funcItj = funcIti->second.find(Input);
            if (funcItj == funcIti->second.end())
            {
                continue;
            }
            else
            {
                SIt = find(S.begin(), S.end(), Input);
                if (SIt != S.end())
                {
                    image.push_back(Q[i]);
                }
            }
        }
    }
    // 对image进行排序和去重
    sort(image.begin(), image.end());
    image.erase(unique(image.begin(), image.end()), image.end());
    return image;
}
// DFA最小化
void DFA::minDFA()
{
    DFA mindfa;
    vector<vector<int>> Pi;
    vector<vector<int>> PiNew;
    vector<int> q1;
    vector<int> q2 = Z;
    vector<int> image;
    // 求差集
    set_difference(Q.begin(), Q.end(), Z.begin(), Z.end(), back_inserter(q1));
    Pi.push_back(q1);
    Pi.push_back(q2);
    bool beenPart;
    do
    {
        beenPart = false;
        PiNew.clear();
        for (int i = 0; i < Pi.size(); i++)
        {
            if (Pi[i].size() == 1)
            {
                PiNew.push_back(Pi[i]);
                continue;
            }
            for (int j = 0; j < Sigma.size(); j++)
            {
                image.clear();
                q1.clear();
                q2.clear();
                image = findImage(Pi[i], Sigma[j]);
                set_intersection(Pi[i].begin(), Pi[i].end(), image.begin(), image.end(), back_inserter(q1));
                if (q2.size() != 0)
                {
                    PiNew.push_back(q1);
                    PiNew.push_back(q2);
                    beenPart = true;
                    cout << beenPart;
                    break;
                }
            }
            if (beenPart)
            {
                i++;
                while (i < Pi.size())
                {
                    PiNew.push_back(Pi[i]);
                    i++;
                }
                break;
            }
        }
        if (beenPart)
        {
            Pi = PiNew;
        }
    } while (beenPart);
    int minQ; // 一个等价类的代表结点
    map<int, map<int, int>>::iterator funci;
    map<int, int>::iterator funcj;
    // cout << "minDFA:" << endl
    //      << "Q:{";
    // for (int i = 0; i < Pi.size(); i++)
    // {
    //     if (Pi[i].size() > 1)
    //     {
    //         // minQ = Pi[i][0]; //选取第一个值为代表值
    //         // mindfa.Q.push_back(minQ);
    //         cout << "{";
    //         for (int j = 0; j < Pi[i].size(); j++)
    //         {
    //             cout << Pi[i][j] << " ";
    //         }
    //         cout << "}";
    //     }
    //     else
    //         cout << ",{" << Pi[i][0] << "}";
    // }
    // cout << "}";
}
// 语法类
class Grammar
{
public:
    vector<string> VN;
    vector<string> VT;
    set<pair<string, vector<string>>> P;
    map<string, set<string>> firstSet;
    map<string, set<string>> followSet;
    map<pair<string, vector<string>>, set<string>> selectSet;
    map<pair<string, string>, vector<string>> preAnTable; // 预测分析表
    string S;
    vector<set<pair<string, vector<string>>>> projectSet; // LR0项目集簇
    NFA nfa;
    DFA dfa;
    LR1AnaTable lr1table;

    // 重写空参构造
    Grammar();
    void printGrammar();
    void setProjectSet();
    void printProjectSet();
    // 初始化LR0b分析表
    void makeLR1Table();
    void printLR1Table();
};
// 重写空参构造
Grammar::Grammar()
{
    string fileName = "F:\\Codes\\Compile_Codes\\input\\grammar.txt";
    // cout << "input file name:";
    // cin >> fileName;
    ifstream fin;
    fin.open(fileName.c_str());
    if (!fin.is_open())
    {
        cout << "file " << fileName << " open failed!" << endl;
        exit(-1);
    }
    int num;
    // input VN
    fin >> num;
    for (int i = 0; i < num; i++)
    {
        string tmp;
        fin >> tmp;
        VN.push_back(tmp);
    }
    // input VT
    fin >> num;
    for (int i = 0; i < num; i++)
    {
        string tmp;
        fin >> tmp;
        VT.push_back(tmp);
    }
    // input P
    fin >> num;
    for (int i = 0; i < num; i++)
    {
        string tmp;
        string left;
        vector<string> right;
        fin >> left;
        fin >> tmp;
        if (tmp == "->")
        {
            char scanner;
            tmp = "";
            do
            {
                scanner = fin.get();
                if (scanner != ' ' && scanner != '\n')
                {
                    tmp.append(1, scanner);
                }
                else if (tmp.size() != 0)
                {
                    right.push_back(tmp);
                    tmp = "";
                }
            } while (scanner != '\n');
        }
        P.insert(make_pair(left, right));
    }
    fin >> S;
    fin.close();
}
void Grammar::printGrammar()
{
    cout << "CFG=(VN,VT,P,S)" << endl;
    // 输出非终结符
    cout << "  VN:";
    vector<string>::iterator vectorIt;
    set<pair<string, vector<string>>>::iterator productionIt;
    for (vectorIt = VN.begin(); vectorIt != VN.end(); vectorIt++)
    {
        cout << " " << *vectorIt;
    }
    cout << endl;

    // 输出终结符
    cout << "  VT:";
    for (vectorIt = VT.begin(); vectorIt != VT.end(); vectorIt++)
    {
        cout << " " << *vectorIt;
    }
    cout << endl;

    // 输出产生式
    cout << "  Production:" << endl;
    int i = 0;
    for (productionIt = P.begin(); productionIt != P.end(); productionIt++, i++)
    {
        cout << "    " << i << ": ";
        cout << (*productionIt).first << " ->";
        for (unsigned j = 0; j < (*productionIt).second.size(); j++)
        {
            cout << " " << (*productionIt).second[j];
        }
        cout << endl;
    }

    // 输出开始符
    cout << "  StartSymbol: " << S << endl;
}
void Grammar::setProjectSet()
{
    int number = 0; // 当前已存在的状态数
    int workIndex = 0;
    set<pair<string, vector<string>>> tmp;
    vector<set<pair<string, vector<string>>>> workSet;
    vector<string> vtAndvn;
    // 初始化
    // 将vt与vn合并
    vtAndvn.insert(vtAndvn.end(), VT.begin(), VT.end());
    vtAndvn.insert(vtAndvn.end(), VN.begin(), VN.end());
    // 计算I0
    for (auto PItem : P)
    {
        if (PItem.second[0] != "epsilon")
        {
            PItem.second.insert(PItem.second.begin(), ".");
            tmp.insert(PItem);
        }
        else
        {
            PItem.second.clear();
            PItem.second.push_back(".");
            tmp.insert(PItem);
        }
    }
    projectSet.push_back(tmp);
    // nfa初始化
    nfa.Q.push_back(number);
    nfa.S.push_back(number);
    for (unsigned i = 0; i < vtAndvn.size(); i++)
    {
        nfa.Sigma.push_back(i);
    }
    // 工作集合
    workSet = projectSet;
    while (workSet.size() != 0)
    {
        auto workItem = *workSet.begin();
        workIndex = isInProSet(projectSet, workItem);
        for (unsigned i = 0; i < vtAndvn.size(); i++)
        {
            string symbol = vtAndvn[i];
            // cout << "current symbol is:" << symbol << endl;
            set<pair<string, vector<string>>> tmp;
            for (auto eachPro : workItem)
            {
                int findsy = find(eachPro.second.begin(), eachPro.second.end(), ".") + 1 - eachPro.second.begin();
                if (eachPro.second[findsy] == symbol)
                {
                    swap(eachPro.second[findsy - 1], eachPro.second[findsy]);
                    tmp.insert(eachPro);
                    if (findsy < eachPro.second.size() - 1)
                    {
                        if (!isInVT(VT, eachPro.second[findsy + 1]))
                        {
                            for (auto item : workItem)
                            {
                                if (item.first == eachPro.second[findsy + 1])
                                {
                                    tmp.insert(item);
                                }
                            }
                        }
                    }
                }
            }
            if (!tmp.empty())
            {
                if (isInProSet(projectSet, tmp) == -1)
                {
                    number++;
                    nfa.Q.push_back(number);
                    nfa.function[workIndex][i].push_back(number);
                    projectSet.push_back(tmp);
                    workSet.push_back(tmp);
                }
                else
                {
                    nfa.function[workIndex][i].push_back(isInProSet(projectSet, tmp));
                }
            }
        }
        workSet.erase(workSet.begin());
    }
    for (auto QItem : nfa.Q)
    {
        if (!isInZ(nfa.function, QItem))
        {
            nfa.Z.push_back(QItem);
        }
    }
    // 根据NFA构建DFA
    dfa = nfa.NFAToDFA();
    // 最小化DFA
    dfa.minDFA();
}
void Grammar::printProjectSet()
{
    cout << "LR(1) 项目集簇" << endl;
    int i = 0;
    for (auto PSItem : projectSet)
    {
        cout << "  I" << i << " :" << endl;
        for (auto setItem = PSItem.begin(); setItem != PSItem.end(); setItem++)
        {
            cout << "    " << (*setItem).first << " -> ";
            for (unsigned j = 0; j < (*setItem).second.size(); j++)
            {
                cout << (*setItem).second[j] << " ";
            }
            cout << endl;
        }
        i++;
    }
    nfa.printNFA();
    dfa.printDFA();
}
void Grammar::makeLR1Table()
{
    // 初始化
    cout << endl;
    for (auto QItem : dfa.Q)
    {
        for (auto SigmaItem : dfa.Sigma)
        {
            if (SigmaItem < VT.size())
            {
                if (isInZ(dfa.Z, QItem))
                {
                    if (S == (*(projectSet[QItem].begin())).first)
                    { // 如果是开始符
                        lr1table.Action[QItem][-1] = "acc";
                    }
                    else
                    {
                        int index = findIngrammar(P, *(projectSet[QItem].begin()));
                        lr1table.Action[QItem][SigmaItem] = "r" + to_string(index);
                        lr1table.Action[QItem][-1] = "r" + to_string(index);
                    }
                }
                else
                    lr1table.Action[QItem][SigmaItem] = "error";
            }
            else
            {
                lr1table.Go[QItem][SigmaItem] = "error";
            }
        }
        if (lr1table.Action[QItem][-1] != "acc" && *(lr1table.Action[QItem][-1].begin()) != 'r')
            lr1table.Action[QItem][-1] = "error";
    }
    for (auto mapItem : dfa.function)
    {
        for (auto mapItem2 : mapItem.second)
        {
            if (mapItem2.first < VT.size())
            {
                if (lr1table.Action[mapItem.first][mapItem2.first] != "error")
                {
                    cout << "该文法不是LR(1)文法！" << endl;
                    exit(-1);
                }
                string tmp = "s" + to_string(mapItem2.second);
                lr1table.Action[mapItem.first][mapItem2.first] = tmp;
            }
            else
            {
                if (lr1table.Go[mapItem.first][mapItem2.first] != "error")
                {
                    cout << "该文法不是LR(1)文法！" << endl;
                    exit(-1);
                }
                string tmp = to_string(mapItem2.second);
                lr1table.Go[mapItem.first][mapItem2.first] = tmp;
            }
        }
    }
    cout << "该文法为LR(1)文法！！" << endl
         << endl;
}
void Grammar::printLR1Table()
{
    ofstream fout("LR1Table.lrtbl");
    if (fout.is_open())
    {
        cout << "保存LR1Table.lrtbl文件中……" << endl;
        for (auto mapItem : lr1table.Action)
        {
            for (auto mapItem2 : mapItem.second)
            {
                if (mapItem2.second != "error")
                {
                    fout << mapItem.first << " " << mapItem2.first << " " << mapItem2.second << endl;
                }
            }
        }
        fout << endl;
        for (auto mapItem : lr1table.Go)
        {
            for (auto mapItem2 : mapItem.second)
            {
                if (mapItem2.second != "error")
                {
                    fout << mapItem.first << " " << mapItem2.first << " " << mapItem2.second << endl;
                }
            }
        }
        cout << "文件转储成功！！" << endl;
        fout.close();
    }
    else
    {
        cout << "error!" << endl;
        exit(-1);
    }
}
int main()
{
    Grammar g;
    g.printGrammar();
    // LR1项目集簇构建
    // NFA构建
    // DFA构建与最小化
    g.setProjectSet();
    g.printProjectSet();
    g.makeLR1Table();
    g.printLR1Table();
    return 0;
}
