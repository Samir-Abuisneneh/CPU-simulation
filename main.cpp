#include<bits/stdc++.h>
using namespace std;
struct PCB{
int processID;
int arTime;
int CPUburst;
int sizeByte;
};
int memorySize, pageSize, quantum, CS;
vector<PCB> process ,processFCFS,processSJF,processRR;

void print(vector<PCB> vec){
    for (int i = 0; i < vec.size(); i++)
    {
        cout<<vec[i].processID<<" "<<vec[i].arTime<<" "<<vec[i].CPUburst<<" "<<vec[i].sizeByte<<"\n";
    }
}

bool compareTime(PCB a, PCB b){ //to sort the FCFS vector based on arrival time
        return a.arTime < b.arTime;
    }
bool compareCPUburst(PCB a, PCB b){ //to sort the SJF vector based on CPU burst time
        return a.CPUburst < b.CPUburst;
    }
void printInfo(vector<PCB> process,vector<int> finishTime,vector<int> waitTime,vector<int> turnaroundTime){
    for (int i = 0; i < process.size(); i++)
    {
        turnaroundTime.push_back(finishTime[i]-process[i].arTime);
    }
    
    for (int i = 0; i < process.size(); i++)
    {
        cout<<"Process "<<process[i].processID<<'\n';
        cout<<"Finish Time = "<<finishTime[i]<<'\n';
        cout<<"Wait Time = "<<waitTime[i]<<'\n';
        cout<<"Turnaround Time = "<<turnaroundTime[i]<<'\n';
        cout<<'\n';
    }
    float avgFinish = accumulate(finishTime.begin(),finishTime.end(),0) / (finishTime.size()*1.0);
    float avgWait = accumulate(waitTime.begin(),waitTime.end(),0) / (waitTime.size()*1.0);
    float avgTurn = accumulate(turnaroundTime.begin(),turnaroundTime.end(),0) / (turnaroundTime.size()*1.0);

    cout<<"Average Finish time = "<<avgFinish<<endl;
    cout<<"Average waiting time = "<<avgWait<<endl;
    cout<<"Average Turnaround time = "<<avgTurn<<endl;
}


void FCFS(){ 
    sort (processFCFS.begin(), processFCFS.end(), compareTime);
    vector<int> finishTime,waitTime,turnaroundTime;   
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
    cout<<"\n";
    printInfo(processFCFS,finishTime,waitTime,turnaroundTime);
}

void SJF(){ //algorithm discussed with omar aburish
    vector <PCB> ready;
    vector <PCB> execOrder;
    vector<int> finishTime,waitTime,turnaroundTime;   
    int progress = 0;
    int elapsed = 0;
    int size = processSJF.size();
    for (int i = 0; i < size; i++){
        for (int j = 0; j < processSJF.size();j++){
            if (processSJF[j].arTime <= elapsed)
            {
                ready.push_back(processSJF[j]);
                sort(ready.begin(), ready.end(),compareCPUburst);
                processSJF.erase(processSJF.begin() + j);
                j--;
            }
                    
        }
        elapsed += ready[0].CPUburst;
        execOrder.push_back(ready[0]);
        ready.erase(ready.begin());
    }
    cout<<'\n';
    for (int i = 0; i < execOrder.size(); i++)
    {    
        cout<<"  p"<<execOrder[i].processID<<"  ";
    }
    cout<<'\n';
    for (int i = 0; i < execOrder.size(); i++)
    {
        cout<<progress<<" ";
        waitTime.push_back(progress - execOrder[i].arTime);
        progress += execOrder[i].CPUburst;
        cout<<progress<<" ";
        finishTime.push_back(progress);
        if (execOrder.size() - i != 1){
        progress += CS; 
        }
    }
    cout<<'\n';
    printInfo(execOrder,finishTime,waitTime,turnaroundTime);

    
}
void roundRobin(){
    vector<PCB> ready;
    queue<PCB> RRqueue;
    vector<int> finishTime(processRR.size()),waitTime(processRR.size()),turnaroundTime;   
    int progress = 0;
    int elapsed = 0;
    int numOfIterations = 0;    
    for (int i = 0; i < processRR.size(); i++)
    {
        double temp = processRR[i].CPUburst / 10.0;
        numOfIterations += ceil(temp);
    }
    for (int i = 0; i <= numOfIterations; i++){
        for (int j = 0; j < processRR.size();j++){
            if (processRR[j].arTime <= elapsed)
            {
                ready.push_back(processRR[j]);
                sort(ready.begin(), ready.end(),compareTime);
                processRR.erase(processRR.begin() + j);
                j--;
            }
        }
        if (numOfIterations - i != 0 && !RRqueue.empty())
        {
            progress += CS;
        }
        if (!RRqueue.empty())
        {
            for (int k = 0; k < ready.size();k++)
            {
                RRqueue.push(ready[k]);
            }
            PCB temp;
            temp = RRqueue.front();
            waitTime[temp.processID] += abs(progress - finishTime[temp.processID]); 
            if (temp.CPUburst - quantum > 0)
            {
                progress += quantum;
            }
            else{
                progress += temp.CPUburst;
            }
            temp.CPUburst -= quantum;
            cout<< "P" << temp.processID <<"\n";
            RRqueue.pop();
            finishTime[temp.processID] = progress; 
            if (temp.CPUburst > 0)
            {
                RRqueue.push(temp);
            }
        }
        else{
            for (int k = 0; k < ready.size();k++)
            {
                progress--;
                RRqueue.push(ready[k]);
            }
        }

        elapsed += quantum;
        cout<<'\n';
        ready.clear();
    }
    for(int i = 0; i < process.size();i++)
    {
        waitTime[i] = waitTime[i] - process[i].arTime;
    }
    printInfo(process,finishTime,waitTime,turnaroundTime);
    
}
int main(){
    ifstream file("processes.txt");
    file >> memorySize >> pageSize >> quantum >> CS;
    while (!file.eof())
    {
        PCB temp;
        file >> temp.processID >>temp.arTime >> temp.CPUburst >>temp.sizeByte;
        process.push_back(temp);
    }
    processFCFS = process;
    processSJF = process;
    processRR = process;
    FCFS();
    SJF();
    roundRobin();
    return 0;
}