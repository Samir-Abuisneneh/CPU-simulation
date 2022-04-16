#include<bits/stdc++.h>
using namespace std;
struct PCB{
int processID;
int arTime;
int CPUburst;
int sizeByte;
};

int memorySize, pageSize, RR, CS;
vector<PCB> process;
bool compareFCFS(PCB a, PCB b){
        return a.arTime < b.arTime;
    }
void FCFS(){
    sort (process.begin(), process.end(), compareFCFS);
}

int main(){
    ifstream file("processes.txt");
    file >> memorySize >> pageSize >> RR >> CS;
    while (!file.eof())
    {
        PCB temp;
        file >> temp.processID >>temp.arTime >> temp.CPUburst >>temp.sizeByte;
        process.push_back(temp);
    }
    return 0;
}