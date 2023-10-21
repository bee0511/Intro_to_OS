#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

#define DEBUG 1

// THIS VERSION CANNOT RUN!!!
// I don't want to debug anymore ;-;

class Process
{
public:
    int AT;  // Arrival time
    int BT;  // Burst time
    int RBT; // Remain burst time
    int CT;  // Completion time
    int TAT; // Turn around time
    int WT;  // Waiting time
    int ID;
    bool Preempt;       // Preempt flag
    bool JustPreempted; // flag for RR

    Process()
    {
        AT = BT = RBT = CT = TAT = WT = 0;
        ID = -1;
        Preempt = false;
    }

    Process(int ID)
    {
        this->ID = ID;
        AT = BT = RBT = CT = TAT = WT;
        Preempt = false;
        JustPreempted = false;
    }

    void printInfo()
    {
        std::cout << "----------" << endl;
        std::cout << "Process: " << ID << endl;
        std::cout << "AT: " << AT << endl;
        std::cout << "BT: " << BT << endl;
        std::cout << "RBT: " << RBT << endl;
        std::cout << "CT: " << CT << endl;
        std::cout << "TAT: " << TAT << endl;
        std::cout << "WT: " << WT << endl;
        std::cout << "Preempt: " << ((Preempt == true) ? "True" : "False") << endl;
        std::cout << "Just Preempt: " << ((JustPreempted == true) ? "True" : "False") << endl;
    }
};

class MultilevelQueue
{

public:
    vector<Process *> v;
    int order; // Order of insertion
    int TQ;    // Time Quantum
    int mode;
    int used_time; // Record for processes in RR used time
    Process *working_process;
    bool SRTFPreempt;
    Process *process_need_to_preempt;
    bool RRPreempt;
    Process *SRTF_previous_process;

    MultilevelQueue();

    MultilevelQueue(int m, int t, int ord) : order(ord), mode(m), TQ(t)
    {
        working_process = nullptr;
        v.clear();
        used_time = 0;
        SRTFPreempt = false;
        RRPreempt = false;
        process_need_to_preempt = nullptr;
        SRTF_previous_process = nullptr;
    }

    // Define a custom comparison operator
    bool operator<(const MultilevelQueue &other) const
    {
        return order > other.order; // Higher order has higher priority
    }

    Process* update();
    Process* FCFS();
    Process* SRTF();
    Process* RR();

    void update_wait_time();
    void printVector()
    {
        for (auto it : v)
        {
            it->printInfo();
        }
    }
    static bool compareProcesses(const Process* p1, const Process* p2) {
    if (p1->RBT < p2->RBT) {
        return true;
    } else if (p1->RBT == p2->RBT) {
        // If RBT is the same, sort by AT in descending order
        return p1->AT > p2->AT;
    }
    return false;
}
};

// Update the Multilevel queue for one time interval and return the process has updated
Process* MultilevelQueue::update()
{
    Process *p;
    update_wait_time();
    switch (mode)
    {
    case 0:
        p = FCFS();
#ifdef DEBUG
// std::cout << "[DEBUG] After FCFS:" << endl;
// printVector();
#endif
        break;
    case 1:
        p = SRTF();
#ifdef DEBUG
// std::cout << "[DEBUG] After SRTF:" << endl;
// printVector();
#endif
        break;
    case 2:
        p = RR();
#ifdef DEBUG
// std::cout << "[DEBUG] After RR:" << endl;
// printVector();
#endif
        break;
    default:
        break;
    }
    return p;
}

void MultilevelQueue::update_wait_time()
{
    for (auto it : v)
    {
        it->WT++;
#ifdef DEBUG
// std::cout << "Process: " << it->ID << ", WT: " << it->WT << endl;
#endif
    }
    return;
}

// Update the process queue based on the FCFS for one time interval
// Return the process which has been processed
Process* MultilevelQueue::FCFS()
{
    // printVector();
    Process *p = v.front();
    p->RBT--;
    p->WT--;
    working_process = p;
    if (p->RBT == 0)
    {
        v.erase(v.begin());
        working_process = nullptr;
    }
    return p;
}

// Update the process queue based on the SRTF for one time interval
// Return the process which has been processed
Process* MultilevelQueue::SRTF()
{
    std::sort(v.begin(), v.end(), MultilevelQueue::compareProcesses);
    Process *p = v.front();
    p->RBT--;
    p->WT--;
    working_process = p;
    if (SRTF_previous_process == nullptr)
        SRTF_previous_process = p;
    if (SRTF_previous_process != nullptr && SRTF_previous_process != p)
    {
        SRTFPreempt = true;
        working_process = nullptr;
        process_need_to_preempt = p;
        SRTF_previous_process = p;
    }
#ifdef DEBUG
    // std::cout << "[DEBUG] Previous ID: " << previous->ID << endl;
    // std::cout << "[DEBUG] Current ID: " << p->ID << endl;
    // std::cout << "[DEBUG] Process RBT: " << p->RBT << endl;
#endif
    if (p->RBT == 0)
    {
#ifdef DEBUG
        std::cout << "[DEBUG] Deleting Process: " << p->ID << endl;
#endif
        v.erase(v.begin()); // Remove the process using the iterator
        working_process = nullptr;
    }
    return p;
}

// Update the process queue based on the RR for one interval
// Return the process which has been processed
Process* MultilevelQueue::RR()
{
    Process *p = v.front();
    p->RBT--;
    p->WT--;
    used_time++;
    if (used_time == TQ && p->RBT > 0)
    {
        used_time = 0;
        RRPreempt = true;
        working_process = nullptr;
        process_need_to_preempt = p;
        return p;
    }
#ifdef DEBUG
    // std::cout << "[DEBUG] Previous ID: " << previous->ID << endl;
    // std::cout << "[DEBUG] Current ID: " << p->ID << endl;
    // std::cout << "[DEBUG] Used time: " << used_time << endl;
    // std::cout << "[DEBUG] Process RBT: " << p->RBT << endl;
#endif
    working_process = p;
    if (p->RBT == 0)
    {
#ifdef DEBUG
        std::cout << "[DEBUG] Deleting Process: " << p->ID << endl;
#endif
        // Find the iterator to the process with matching ID
        auto it = std::find_if(v.begin(), v.end(), [p](const Process *process)
                               { return process->ID == p->ID; });

        if (it != v.end())
        {
            // Erase the process with matching ID from the vector
            v.erase(it);
        }
        used_time = 0;
        working_process = nullptr;
    }
    return p;
}

bool CompareProcessesByID(const Process *p1, const Process *p2)
{
    return p1->ID < p2->ID;
}

void disableAllJustPreempted(priority_queue<MultilevelQueue> &pq)
{
    vector<MultilevelQueue> tmpMQ;

    while (!pq.empty())
    {
        tmpMQ.push_back(pq.top());
        pq.pop();
    }
    for (auto it : tmpMQ)
    {
        for (int i = 0; i < it.v.size(); i++)
        {
            it.v[i]->JustPreempted = false;
        }
        pq.push(it);
    }
    tmpMQ.clear();
    return;
}

void printAllQueueStatus(priority_queue<MultilevelQueue> pq)
{
    vector<MultilevelQueue> tmpMQ;
    while (!pq.empty())
    {
        tmpMQ.push_back(pq.top());
        pq.pop();
    }
    for (auto it : tmpMQ)
    {
        std::cout << "=====================" << endl;
        std::cout << "[DEBUG] Queue number: " << it.order << "'s info: " << endl;
        if (it.working_process != nullptr)
            std::cout << "[DEBUG] Queue working process ID: " << it.working_process->ID << endl;
        else
            std::cout << "[DEBUG] No working process!" << endl;
        switch (it.mode)
        {
        case 0:
            std::cout << "[DEBUG] FCFS mode" << endl;
            break;
        case 1:
            std::cout << "[DEBUG] SRTF mode" << endl;
            break;
        case 2:
            std::cout << "[DEBUG] RR mode" << endl;
            std::cout << "[DEBUG] Time Quantum: " << it.TQ << endl;
            break;
        default:
            break;
        }
        it.printVector();
    }
    return;
}

bool checkAllEmpty(priority_queue<MultilevelQueue> pq)
{
    bool emptyFlag = true;
    vector<MultilevelQueue> tmpMQ;
    while (!pq.empty())
    {
        tmpMQ.push_back(pq.top());
        pq.pop();
    }
    for (auto it : tmpMQ)
    {
        if (it.v.empty() == false)
        {
            emptyFlag = false;
        }
        pq.push(it);
    }
    tmpMQ.clear();
    return emptyFlag;
}

void newProcessCome(priority_queue<MultilevelQueue> &pq, queue<Process *> &processes, int time)
{
    if (processes.empty())
        return;
    vector<MultilevelQueue> tmpMQ;
    while (!pq.empty())
    {
        tmpMQ.push_back(pq.top());
        pq.pop();
    }

    Process *new_process;
    new_process = processes.front();
    // New process arrives
    if (new_process->AT == time)
    {

        tmpMQ[0].v.push_back(new_process); // push the process into the multilevel queue
        processes.pop();                   // remove the process since it has arrived
        if (tmpMQ[0].v.size() >= 2 && tmpMQ[0].v[tmpMQ[0].v.size() - 2]->JustPreempted == true)
        {
            // Used for RR, when pushing into the vector, put the larger arrival time first
            if (tmpMQ[0].v[tmpMQ[0].v.size() - 2]->AT < tmpMQ[0].v[tmpMQ[0].v.size() - 1]->AT)
            {
                std::swap(tmpMQ[0].v[tmpMQ[0].v.size() - 2], tmpMQ[0].v[tmpMQ[0].v.size() - 1]);
            }
        }
        //         if (tmpMQ.size() >= 2 && tmpMQ[0].v.size() == 1)
        //         {
        //             // Preempt lower queue's process when new process coming
        //             bool lower_queue_exist_process = false;
        //             int queue_id = -1;
        //             for (int i = 1; i < tmpMQ.size(); i++)
        //             {
        //                 if (!tmpMQ[i].v.empty())
        //                 {
        //                     lower_queue_exist_process = true;
        //                     queue_id = i;
        //                     break;
        //                 }
        //             }
        //             if (lower_queue_exist_process && tmpMQ[queue_id].working_process != nullptr)
        //             {
        // #ifdef DEBUG
        //                 std::cout << "[DEBUG] Setting Queue " << queue_id << "'s process " << tmpMQ[queue_id].previous->ID << " to true!" << endl;
        // #endif
        //                 tmpMQ[queue_id].working_process->Preempt = true;
        //             }
        //         }
    }

    for (auto it : tmpMQ)
    {
        for (int i = 0; i < it.v.size(); i++)
        {
            it.v[i]->JustPreempted = false;
        }
        pq.push(it);
    }
    tmpMQ.clear();
    return;
}

pair<Process *, int> updateProcessQueue(priority_queue<MultilevelQueue> &pq)
{
    vector<MultilevelQueue> tmpMQ;
    MultilevelQueue updated_queue = pq.top();
    Process *updated_process;
    while (!pq.empty())
    {
        tmpMQ.push_back(pq.top());
        pq.pop();
    }
    int process_queue_priority = -1;
    for (int i = 0; i < tmpMQ.size(); i++)
    {
        if (!tmpMQ[i].v.empty())
        {
            updated_process = tmpMQ[i].update();
            updated_queue = tmpMQ[i];
            process_queue_priority = i;
            break;
        }
    }

    for (int i = 0; i < tmpMQ.size(); i++)
    {
        if (tmpMQ[i].order != updated_queue.order)
        {
            // tmpMQ[i].working_process = nullptr;
            tmpMQ[i].update_wait_time();
        }
    }
    for (auto it : tmpMQ)
    {
        pq.push(it);
    }
    tmpMQ.clear();
    return make_pair(updated_process, process_queue_priority);
}

void preemptHandler(priority_queue<MultilevelQueue> &pq, int process_queue_priority)
{
    vector<MultilevelQueue> tmpMQ;
    while (!pq.empty())
    {
        tmpMQ.push_back(pq.top());
        pq.pop();
    }
    for (int i = 0; i < tmpMQ.size(); i++)
    {
        if (tmpMQ[i].SRTFPreempt) // Handle SRTF Preempt
        {
            tmpMQ[i].process_need_to_preempt->Preempt = true;
            tmpMQ[i].SRTFPreempt = false;
            for (int j = 0; j < tmpMQ[i].v.size(); j++)
            {
                if (tmpMQ[i].v[j]->Preempt == true)
                {
                    if (i != tmpMQ.size() - 1)
                    { // exist other level Queue
                        tmpMQ[i].v[j]->Preempt = false;
                        tmpMQ[i].v[j]->JustPreempted = true;
                        tmpMQ[i + 1].v.push_back(tmpMQ[i].v[j]);
                        auto it = std::find(tmpMQ[i].v.begin(), tmpMQ[i].v.end(), tmpMQ[i].v[j]);
                        tmpMQ[i].v.erase(it);
                        tmpMQ[i].SRTF_previous_process = nullptr;
                        tmpMQ[i].working_process = nullptr;
                    }
                    else
                    {
                        Process *tmp = tmpMQ[i].v[j];
                        tmp->Preempt = false;
                        tmp->JustPreempted = true;
                        auto it = std::find(tmpMQ[i].v.begin(), tmpMQ[i].v.end(), tmpMQ[i].v[j]);
                        tmpMQ[i].v.erase(it);
                        tmpMQ[i].v.push_back(tmp);
                        tmpMQ[i].SRTF_previous_process = tmp;
                        tmpMQ[i].working_process = nullptr;
                    }
                }
            }
        }
        if (tmpMQ[i].RRPreempt) // Handle RR Preempt
        {
            tmpMQ[i].process_need_to_preempt->Preempt = true;
            tmpMQ[i].RRPreempt = false;
            for (int j = 0; j < tmpMQ[i].v.size(); j++)
            {
                if (tmpMQ[i].v[j]->Preempt == true)
                {
                    if (i != tmpMQ.size() - 1)
                    { // exist other level Queue
                        tmpMQ[i].v[j]->Preempt = false;
                        tmpMQ[i].v[j]->JustPreempted = true;
                        tmpMQ[i + 1].v.push_back(tmpMQ[i].v[j]);
                        auto it = std::find(tmpMQ[i].v.begin(), tmpMQ[i].v.end(), tmpMQ[i].v[j]);
                        tmpMQ[i].v.erase(it);
                        tmpMQ[i].working_process = nullptr;
                    }
                    else
                    {
                        Process *tmp = tmpMQ[i].v[j];
                        tmp->Preempt = false;
                        tmp->JustPreempted = true;
                        auto it = std::find(tmpMQ[i].v.begin(), tmpMQ[i].v.end(), tmpMQ[i].v[j]);
                        tmpMQ[i].v.erase(it);
                        tmpMQ[i].v.push_back(tmp);
                        tmpMQ[i].working_process = nullptr;
                    }
                }
            }
        }
        if (tmpMQ[i].working_process != nullptr && process_queue_priority > i)
        { // Handle higher Priority Preempt
            tmpMQ[i].working_process->Preempt = true;
            for (int j = 0; j < tmpMQ[i].v.size(); j++)
            {
                if (tmpMQ[i].v[j]->Preempt == true)
                {
                    if (i != tmpMQ.size() - 1)
                    { // exist other level Queue
                        tmpMQ[i].v[j]->Preempt = false;
                        tmpMQ[i].v[j]->JustPreempted = true;
                        tmpMQ[i + 1].v.push_back(tmpMQ[i].v[j]);
                        auto it = std::find(tmpMQ[i].v.begin(), tmpMQ[i].v.end(), tmpMQ[i].v[j]);
                        tmpMQ[i].v.erase(it);
                        tmpMQ[i].working_process = nullptr;
                    }
                    else
                    {
                        Process *tmp = tmpMQ[i].v[j];
                        tmp->Preempt = false;
                        tmp->JustPreempted = true;
                        auto it = std::find(tmpMQ[i].v.begin(), tmpMQ[i].v.end(), tmpMQ[i].v[j]);
                        tmpMQ[i].v.erase(it);
                        tmpMQ[i].v.push_back(tmp);
                        tmpMQ[i].working_process = nullptr;
                    }
                }
            }
        }
        //     for (int j = 0; j < tmpMQ[i].v.size(); j++)
        //     {
        //         if (tmpMQ[i].v[j]->Preempt == true)
        //         {
        //             if (i != tmpMQ.size() - 1)
        //             { // exist other level Queue
        //                 tmpMQ[i].v[j]->Preempt = false;
        //                 tmpMQ[i].v[j]->JustPreempted = true;
        //                 tmpMQ[i + 1].v.push_back(tmpMQ[i].v[j]);
        //                 auto it = std::find(tmpMQ[i].v.begin(), tmpMQ[i].v.end(), tmpMQ[i].v[j]);
        //                 tmpMQ[i].v.erase(it);
        //             }
        //             else
        //             {
        //                 Process *tmp = tmpMQ[i].v[j];
        //                 tmp->Preempt = false;
        //                 tmp->JustPreempted = true;
        //                 auto it = std::find(tmpMQ[i].v.begin(), tmpMQ[i].v.end(), tmpMQ[i].v[j]);
        //                 tmpMQ[i].v.erase(it);
        //                 tmpMQ[i].v.push_back(tmp);
        //             }

        //             if (tmpMQ[i].mode == 2 && tmpMQ[i].used_time == 0 && tmpMQ[i].working_process != nullptr)
        //             {
        //                 tmpMQ[i].working_process = nullptr;
        //             }
        //         }
        //     }
        tmpMQ[i].process_need_to_preempt = nullptr;
    }
    for (auto it : tmpMQ)
    {
        pq.push(it);
    }
    tmpMQ.clear();
    return;
}

void deleteQueueHandler(priority_queue<MultilevelQueue> &pq, queue<Process *> &processes)
{
    vector<MultilevelQueue> tmpMQ;
    while (!pq.empty())
    {
        tmpMQ.push_back(pq.top());
        pq.pop();
    }
    if (!tmpMQ[0].v.empty() || !processes.empty())
    {
        pq.push(tmpMQ[0]);
    }
    for (int i = 1; i < tmpMQ.size(); i++)
    {
        pq.push(tmpMQ[i]);
    }
    return;
}

int main()
{
    int N, M;
    cin >> N >> M;
    priority_queue<MultilevelQueue> pq;

    for (int i = 0; i < N; i++)
    {
        int mode, TQ;
        cin >> mode >> TQ;
        pq.push(MultilevelQueue(mode, TQ, i));
    }

    queue<Process *> processes;
    for (int i = 0; i < M; i++)
    {
        Process *tmp = new Process(i);
        cin >> tmp->AT >> tmp->BT;
        tmp->RBT = tmp->BT;
        tmp->Preempt = false;
        tmp->WT = 0;
        tmp->CT = 0;
        tmp->TAT = 0;
        processes.push(tmp);
    }

    int time = 0;
    vector<Process *> finish;
    Process *currentProcess = nullptr;
    while (true)
    {
#ifdef DEBUG
        std::cout << "[DEBUG] Time: " << time << endl;
        fgetc(stdin);
#endif

        // New processes have not arrived and all queues are empty
        if (checkAllEmpty(pq) && processes.front()->AT > time)
        {
            disableAllJustPreempted(pq);
            time++;
            continue;
        }

        newProcessCome(pq, processes, time);
        pair<Process *, int> process_info;
        process_info = updateProcessQueue(pq);
        currentProcess = process_info.first;
        int process_queue_priority = process_info.second;
        preemptHandler(pq, process_queue_priority);

        // The highest priority multilevel queue is not empty or not all processes has arrived
        deleteQueueHandler(pq, processes);
        // Process has finished
        if (currentProcess->RBT == 0)
        {
            currentProcess->CT = time + 1;
            currentProcess->TAT = currentProcess->CT - currentProcess->AT;
            finish.push_back(currentProcess);
        }
        cout << "PQ.size(): " << pq.size() << endl;
        // All multilevel queues are empty
        if (pq.empty())
        {
            break;
        }
        time++; // Update timer
#ifdef DEBUG
        printAllQueueStatus(pq);
        std::cout << "*******************************" << endl;
#endif
    }

    int total_wait_time = 0;
    int total_turnaround_time = 0;

    std::sort(finish.begin(), finish.end(), CompareProcessesByID);
    for (Process *p : finish)
    {
#ifdef DEBUG
        p->printInfo();
#endif
        std::cout << p->WT << " " << p->TAT << endl;
        total_wait_time += p->WT;
        total_turnaround_time += p->TAT;
        delete p;
    }
    std::cout << total_wait_time << endl;
    std::cout << total_turnaround_time << endl;

    return 0;
}
