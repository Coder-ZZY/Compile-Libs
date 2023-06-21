#include <iostream>
#include <set>
using namespace std;
int main(){
    int test[] = {12,23,345,45};
    set<int> test2(test,test+4);
    for(auto testItem: test2){
        cout << testItem << " ";
    }
}