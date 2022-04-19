#include <bits/stdc++.h>
#include<thread>
#include "TextTable.h"
using namespace std;
struct PCB
{
    int processID;
    int arTime;
    int CPUburst;
    int sizeByte;
};
int memorySize, pageSize, quantum, CS;
vector<PCB> process, processFCFS, processSJF, processRR;

void print(vector<PCB> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec[i].processID << " " << vec[i].arTime << " " << vec[i].CPUburst << " " << vec[i].sizeByte << "\n";
    }
}

bool compareTime(PCB a, PCB b)
{ // to sort the FCFS vector based on arrival time
    return a.arTime < b.arTime;
}

bool compareCPUburst(PCB a, PCB b)
{ // to sort the SJF vector based on CPU burst time
    return a.CPUburst < b.CPUburst;
}

void printInfo(vector<PCB> process, vector<int> finishTime, vector<int> waitTime, vector<int> turnaroundTime, int progress)
{
    int optimalCPUusage = 0;
    int CPUusageOfMethod = 0;
    for (int i = 0; i < process.size(); i++)
    {
        turnaroundTime.push_back(finishTime[i] - process[i].arTime);
    }
    for (int i = 0; i < process.size(); i++)
    {
        optimalCPUusage += process[i].CPUburst;
    }

    for (int i = 0; i < process.size(); i++)
    {
        cout << "Process " << process[i].processID << '\n';
        cout << "Finish Time = " << finishTime[i] << '\n';
        cout << "Wait Time = " << waitTime[i] << '\n';
        cout << "Turnaround Time = " << turnaroundTime[i] << '\n';
        cout << '\n';
    }
    float avgFinish = accumulate(finishTime.begin(), finishTime.end(), 0) / (finishTime.size() * 1.0);
    float avgWait = accumulate(waitTime.begin(), waitTime.end(), 0) / (waitTime.size() * 1.0);
    float avgTurn = accumulate(turnaroundTime.begin(), turnaroundTime.end(), 0) / (turnaroundTime.size() * 1.0);

    cout << "Average Finish time = " << avgFinish << endl;
    cout << "Average waiting time = " << avgWait << endl;
    cout << "Average Turnaround time = " << avgTurn << endl;
    cout << "CPU Utilization = " << (optimalCPUusage / (progress * 1.0)) * 100 << "%\n";
}

void FCFS()
{
    TextTable table('-', '|', 'x');
    cout << "\n/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\ FCFS /\\/\\/\\/\\/\\/\\/\\/\\/\\/\\\n";
    string tableInput;
    sort(processFCFS.begin(), processFCFS.end(), compareTime);
    vector<int> finishTime, waitTime, turnaroundTime;
    int progress = 0;
    for (int i = 0; i < processFCFS.size(); i++)
    {
        tableInput = (to_string(processFCFS[i].processID));
        table.add("P" + tableInput + " ");
        if (processFCFS.size() - i != 1)
        {
            table.add("CS");
        }
    }
    table.endOfRow();
    for (int i = 0; i < processFCFS.size(); i++)
    {
        tableInput = "";
        tableInput = to_string(progress);
        waitTime.push_back(progress - processFCFS[i].arTime);
        progress += processFCFS[i].CPUburst;
        tableInput += (" " + to_string(progress));
        table.add(tableInput);
        finishTime.push_back(progress);
        if (processFCFS.size() - i != 1)
        {
            progress += CS;
            tableInput = (to_string(progress) + " " + to_string(progress + CS));
            table.add(tableInput);
        }
    }
    table.endOfRow();
    cout << table;
    cout << '\n';
    printInfo(processFCFS, finishTime, waitTime, turnaroundTime, progress);
}

void SJF()
{ // algorithm discussed with omar aburish
    cout << "\n/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\ SJF /\\/\\/\\/\\/\\/\\/\\/\\/\\/\\\n";
    TextTable table('-', '|', 'x');
    vector<PCB> ready;
    vector<PCB> execOrder;
    vector<int> finishTime, waitTime, turnaroundTime;
    int progress = 0;
    int elapsed = 0;
    int size = processSJF.size();
    string tableInput;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < processSJF.size(); j++)
        {
            if (processSJF[j].arTime <= elapsed)
            {
                ready.push_back(processSJF[j]);
                sort(ready.begin(), ready.end(), compareCPUburst);
                processSJF.erase(processSJF.begin() + j);
                j--;
            }
        }
        elapsed += ready[0].CPUburst;
        execOrder.push_back(ready[0]);
        ready.erase(ready.begin());
    }
    for (int i = 0; i < execOrder.size(); i++)
    {
        tableInput = (to_string(execOrder[i].processID));
        table.add("P" + tableInput + " ");
        if (execOrder.size() - i != 1)
        {
            table.add("CS");
        }
    }
    table.endOfRow();
    for (int i = 0; i < execOrder.size(); i++)
    {
        tableInput = "";
        tableInput = to_string(progress);
        waitTime.push_back(progress - execOrder[i].arTime);
        progress += execOrder[i].CPUburst;
        tableInput += (" " + to_string(progress));
        table.add(tableInput);
        finishTime.push_back(progress);
        if (execOrder.size() - i != 1)
        {
            progress += CS;
            tableInput = (to_string(progress) + " " + to_string(progress + CS));
            table.add(tableInput);
        }
    }
    table.endOfRow();
    cout << table;
    cout << '\n';
    printInfo(execOrder, finishTime, waitTime, turnaroundTime, progress);
}

void roundRobin()
{
    cout << "\n/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\ RoundRobin /\\/\\/\\/\\/\\/\\/\\/\\/\\/\\\n";
    TextTable table('-', '|', 'x');
    vector<PCB> ready;
    queue<PCB> RRqueue;
    vector<int> finishTime(processRR.size()), waitTime(processRR.size()), turnaroundTime;
    int progress = 0;
    int elapsed = 0;
    int numOfIterations = 0;
    vector<string> tableInput_processNames;
    vector<string> tableInput_processValues;
    tableInput_processValues.push_back(to_string(0));

    string tableInput;
    for (int i = 0; i < processRR.size(); i++)
    {
        double temp = processRR[i].CPUburst / 10.0;
        numOfIterations += ceil(temp);
    }
    for (int i = 0; i <= numOfIterations; i++)
    {
        for (int j = 0; j < processRR.size(); j++)
        {
            if (processRR[j].arTime <= elapsed)
            {
                ready.push_back(processRR[j]);
                sort(ready.begin(), ready.end(), compareTime);
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
            for (int k = 0; k < ready.size(); k++)
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
            else
            {
                progress += temp.CPUburst;
            }
            temp.CPUburst -= quantum;
            tableInput_processNames.push_back("P" + to_string(temp.processID));
            RRqueue.pop();

            tableInput_processValues.push_back(to_string(progress));

            if (!RRqueue.empty())
            {
                tableInput_processNames.push_back("CS");
                tableInput_processValues.push_back(to_string(progress + CS));
            }
            finishTime[temp.processID] = progress;
            if (temp.CPUburst > 0)
            {
                RRqueue.push(temp);
            }
        }
        else
        {
            for (int k = 0; k < ready.size(); k++)
            {
                progress--;
                RRqueue.push(ready[k]);
            }
        }

        elapsed += quantum;
        ready.clear();
    }
    for (int i = 0; i < process.size(); i++) // To subtract the Arrival time from the calculated wait time
    {
        waitTime[i] = waitTime[i] - process[i].arTime;
    }

    for (int i = 0; i < tableInput_processNames.size(); i++)
    {
        table.add(tableInput_processNames[i]);
    }
    table.endOfRow();
    for (int i = 1; i < tableInput_processValues.size(); i++)
    {
        string temp;
        temp = ((tableInput_processValues[i - 1]) + " " + tableInput_processValues[i]);
        table.add(temp);
    }
    table.endOfRow();
    cout << table << '\n';
    printInfo(process, finishTime, waitTime, turnaroundTime, progress);
}

int main()
{
    ifstream file("processes.txt");
    if (!file)
    {
        cout<<"processes.txt not found";
        return 1;
    }
    file >> memorySize >> pageSize >> quantum >> CS;
    while (!file.eof())
    {
        PCB temp;
        file >> temp.processID >> temp.arTime >> temp.CPUburst >> temp.sizeByte;
        process.push_back(temp);
    }
    processFCFS = process;
    processSJF = process;
    processRR = process;
    thread t1(FCFS);
    t1.join();
    thread t2(SJF);
    t2.join();
    thread t3(roundRobin);
    t3.join();
    return 0;
}