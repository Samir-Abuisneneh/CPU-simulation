#include<bits/stdc++.h>
using namespace std;
struct PCB{
int processID;
int arTime;
int CPUburst;
int sizeByte;
};
int memorySize, pageSize, RR, CS;





vector<PCB> process ,processFCFS,processSJF,processRR;
void print(vector<PCB> vec){
    for (int i = 0; i < vec.size(); i++)
    {
        cout<<vec[i].processID<<" "<<vec[i].arTime<<" "<<vec[i].CPUburst<<" "<<vec[i].sizeByte<<"\n";
    }
}

bool compareFCFS(PCB a, PCB b){ //to sort the FCFS vector based on arrival time
        return a.arTime < b.arTime;
    }
bool compareSJF(PCB a, PCB b){ //to sort the SJF vector based on CPU burst time
        return a.CPUburst < b.CPUburst;
    }
void FCFS(){ 
    sort (processFCFS.begin(), processFCFS.end(), compareFCFS);
    vector<int> finishTime,waitTime;   
    int progress = 0;
    for (int i = 0; i < processFCFS.size(); i++)
    {    
        cout<<"  p"<<processFCFS[i].processID<<"  ";
    }
    cout<<endl;
    for (int i = 0; i < processFCFS.size(); i++)
    {
        cout<<progress<<" ";
        waitTime.push_back(progress - processFCFS[i].arTime);
        progress += processFCFS[i].CPUburst;
        cout<<progress<<" ";
        finishTime.push_back(progress);
        if (processFCFS.size() - i != 1){
        progress += CS; 
        }
    }
    cout<<"\n\n";
 
    for (int i = 0; i < processFCFS.size(); i++)
    {
        cout<<"Process "<<processFCFS[i].processID<<'\n';
        cout<<"Finish Time = "<<finishTime[i]<<'\n';
        cout<<"Wait Time = "<<waitTime[i]<<'\n';
        cout<<"Turnaround Time = "<<finishTime[i]-processFCFS[i].arTime<<'\n';
        cout<<'\n';
    }



}
void SJF(){ //algorithm discussed with omar aburish
    vector <PCB> ready;
    vector <PCB> execOrder;
    int elapsed = 0;
    int size = processSJF.size();
    for (int i = 0; i < size; i++){
        for (int j = 0; j < processSJF.size();j++){
            if (processSJF[j].arTime <= elapsed)
            {
                ready.push_back(processSJF[j]);
                sort(ready.begin(), ready.end(),compareSJF);
                processSJF.erase(processSJF.begin() + j);
                j--;
            }
                    
        }
        elapsed += ready[0].CPUburst;
        execOrder.push_back(ready[0]);
        ready.erase(ready.begin());
    }
    print(execOrder);
    
    
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
    processFCFS = process;
    processSJF = process;
    processRR = process;
    //FCFS();
    SJF();
    return 0;
}