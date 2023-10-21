#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

#define DEBUG 1

class Process{
public:
    int AT; // Arrival time
    int BT; // Burst time
    int RBT; // Remain burst time
    int CT; // Completion time
    int TAT; // Turn around time
    int WT; // Waiting time
    int ID;
    bool Preempt; // Preempt flag
    bool JustPreempted; // flag for RR

    Process(){
        AT = BT = RBT = CT = TAT = WT = 0;
        ID = -1;
        Preempt = false;
    }

    Process(int ID){
        this->ID = ID;
        AT = BT = RBT = CT = TAT = WT;
        Preempt = false;
        JustPreempted = false;
    }

    void printInfo(){
        cout << "----------" << endl;
        cout << "Process: " << ID << endl;
        cout << "AT: " << AT << endl;
        cout << "BT: " << BT << endl;
        cout << "RBT: " << RBT << endl;
        cout << "CT: " << CT << endl;
        cout << "TAT: " << TAT << endl;
        cout << "WT: " << WT << endl;
        cout << "Preempt: " << ((Preempt == true) ? "True" : "False") << endl;
        cout << "Just Preempt: " << ((JustPreempted == true) ? "True" : "False") << endl;
    }
};

class MultilevelQueue {

public:
    vector<Process*> v;
    int order;  // Order of insertion
    int TQ; // Time Quantum
    int mode;
    int used_time; // Record for processes in RR used time
    Process* previous;

    MultilevelQueue();

    MultilevelQueue(int m, int t, int ord) : order(ord), mode(m), TQ(t) {
        previous = nullptr;
        v.clear();
        used_time = 0;
    }

    // Define a custom comparison operator
    bool operator<(const MultilevelQueue& other) const {
        return order > other.order;  // Higher order has higher priority
    }

    Process* update();
    Process* FCFS();
    Process* SRTF();
    Process* RR();

    void updateWaitTime();
    void printVector(){
        for(auto it: v){
            it->printInfo();
        }
    }
};

// Update the Multilevel queue for one time interval and return the process has updated
Process* MultilevelQueue::update(){
    Process* p;
        switch(mode){
            case 0:
                p = FCFS();
                #ifdef DEBUG
                // cout << "[DEBUG] After FCFS:" << endl;
                // printVector();
                #endif
                break;
            case 1:
                p = SRTF();
                #ifdef DEBUG
                // cout << "[DEBUG] After SRTF:" << endl;
                // printVector();
                #endif
                break;
            case 2:
                p = RR();
                #ifdef DEBUG
                // cout << "[DEBUG] After RR:" << endl;
                // printVector();
                #endif
                break;
            default:
                break;
        }
    return p;
}

void MultilevelQueue::updateWaitTime(){
    for(auto it: v){
        it->WT++;
        #ifdef DEBUG
        // cout << "Process: " << it->ID << ", WT: " << it->WT << endl;
        #endif
    }
    return;
}

// Update the process queue based on the FCFS for one time interval
// Return the process which has been processed
Process* MultilevelQueue::FCFS(){
    // printVector();
    Process* p = v.front();
    p->RBT--;
    p->WT--;
    if(p->RBT == 0) {
        v.erase(v.begin());
        p->WT++;
    }
    updateWaitTime();
    return p;
}
    
// Update the process queue based on the SRTF for one time interval
// Return the process which has been processed
Process* MultilevelQueue::SRTF(){
    Process* p;
    int min_remaining_time = INT_MAX;
    vector<Process*>::iterator selected_process = v.end(); // Initialize with an invalid iterator

    // Find the highest priority process in the vector
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (min_remaining_time > (*it)->RBT) {
            min_remaining_time = (*it)->RBT;
            p = *it;
            selected_process = it; // Update the selected_process iterator
        }
        // If have the same remaining time, choose the larger arrival time
        else if (min_remaining_time == (*it)->RBT) {
            if (p->AT > (*it)->AT) {
                p = *it;
                selected_process = it; // Update the selected_process iterator
            }
        }
    }
    if (previous == nullptr) {
        previous = p;
    }
    if (p != previous) {
        previous->Preempt = true;
    }
    p->RBT--;
    p->WT--;
    #ifdef DEBUG
    cout << "[DEBUG] Previous ID: " << previous->ID << endl;
    cout << "[DEBUG] Current ID: " << p->ID << endl;
    cout << "[DEBUG] Process RBT: " << p->RBT << endl;
    #endif
    if (p->RBT == 0) {
        #ifdef DEBUG
        cout << "[DEBUG] Deleting Process: " << p->ID << endl;
        #endif 
        v.erase(selected_process); // Remove the process using the iterator
        p->WT++;
    }
    updateWaitTime();
    return p;
}
    
// Update the process queue based on the RR for one interval
// Return the process which has been processed
Process* MultilevelQueue::RR(){
    Process* p = v.front();
    if (previous == nullptr){
        previous = p;
        used_time = 0;
    }    
    if (p != previous) {
        previous->Preempt = true;
    }
    
    p->RBT--;
    p->WT--;
    used_time++;
    if(used_time == TQ && p->RBT > 0){
        previous = p;
        if(v.size() > 1){
            p = v[1];
        }
        else p = v[0];
        used_time = 0;
        previous->Preempt = true;
    }
    #ifdef DEBUG
    cout << "[DEBUG] Previous ID: " << previous->ID << endl;
    cout << "[DEBUG] Current ID: " << p->ID << endl;
    cout << "[DEBUG] Used time: " << used_time << endl;
    cout << "[DEBUG] Process RBT: " << p->RBT << endl;
    #endif
    if(p->RBT == 0){
        #ifdef DEBUG
        cout << "[DEBUG] Deleting Process: " << p->ID << endl;
        #endif 
        // Find the iterator to the process with matching ID
        auto it = std::find_if(v.begin(), v.end(), [p](const Process* process) {
            return process->ID == p->ID;
        });

        if (it != v.end()) {
        // Erase the process with matching ID from the vector
            v.erase(it);
        }
        used_time = 0;
        p->WT++;
    }
    updateWaitTime();
    return p;
}

bool CompareProcessesByID(const Process* p1, const Process* p2) {
    return p1->ID < p2->ID;
}

void print_all_queue(priority_queue<MultilevelQueue> pq){
    vector<MultilevelQueue> tmpMQ;
    while(!pq.empty()){
        tmpMQ.push_back(pq.top());
        pq.pop();
    }
    for(auto it : tmpMQ){
        cout << "=====================" << endl;
        cout << "[DEBUG] Queue number: " << it.order << "'s info: " << endl;
        if(it.previous != nullptr)cout << "[DEBUG] Queue previous process ID: " << it.previous->ID << endl;
        else cout << "[DEBUG] No previous!" << endl;
        switch (it.mode)
        {
        case 0:
            cout << "[DEBUG] FCFS mode" << endl;
            break;
        case 1:
            cout << "[DEBUG] SRTF mode" << endl;
            break;
        case 2:
            cout << "[DEBUG] RR mode" << endl;
            cout << "[DEBUG] Time Quantum: " << it.TQ << endl;
            break;
        default:
            break;
        }
        it.printVector();
    }
    return;
}

int main() {
    int N, M;
    cin >> N >> M;
    priority_queue<MultilevelQueue> pq;

    for (int i = 0; i < N; i++) {
        int mode, TQ;
        cin >> mode >> TQ;
        pq.push(MultilevelQueue(mode, TQ, i));
    }

    queue<Process*> processes;
    for (int i = 0; i < M; i++) {
        Process* tmp = new Process(i);
        cin >> tmp->AT >> tmp->BT;
        tmp->RBT = tmp->BT;
        tmp->Preempt = false;
        tmp->WT = 0;
        tmp->CT = 0;
        tmp->TAT = 0;
        processes.push(tmp);
    }
    
    int time = 0;
    vector<Process*> finish;
    while(true){
        #ifdef DEBUG
        cout << "[DEBUG] Time: " << time << endl;
        fgetc(stdin);  
        #endif
        //check whether all queues are empty
        bool emptyFlag = true;
        vector<MultilevelQueue> tmpMQ;
        while(!pq.empty()){
            tmpMQ.push_back(pq.top());
            pq.pop();
        }
        for(auto it: tmpMQ){
            if(it.v.empty() == false){
                emptyFlag = false;
            }
            pq.push(it);
        }
        tmpMQ.clear();

        //If all queue empty and no new process, skip
        if(emptyFlag && processes.front()->AT > time){
            while(!pq.empty()){
                tmpMQ.push_back(pq.top());
                pq.pop();
            }
            for(auto it: tmpMQ){
                for(int i = 0 ; i < it.v.size() ; i++){
                    it.v[i]->JustPreempted = false;
                }
                pq.push(it);
            }
            tmpMQ.clear();
            time++;
            continue;
        }


        while(!pq.empty()){
            tmpMQ.push_back(pq.top());
            pq.pop();
        }
        // check whether new process come
        if(!processes.empty()){
            Process* new_process;
            new_process = processes.front();
            // New process arrives
            if(new_process->AT == time){
                tmpMQ[0].v.push_back(new_process); // push the process into the multilevel queue
                processes.pop(); // remove the process since it has arrived
                if(tmpMQ[0].v.size() >= 2 && tmpMQ[0].v[tmpMQ[0].v.size() - 2]->JustPreempted == true){
                    // Used for RR, when pushing into the vector, put the larger arrival time first
                    if(tmpMQ[0].v[tmpMQ[0].v.size() - 2]->AT < tmpMQ[0].v[tmpMQ[0].v.size() - 1]->AT){
                        std::swap(tmpMQ[0].v[tmpMQ[0].v.size() - 2], tmpMQ[0].v[tmpMQ[0].v.size() - 1]);
                    }
                }
                if(tmpMQ.size() >= 2 && tmpMQ[0].v.size() == 1){
                    // Preempt lower queue's process when new process coming
                    bool lower_queue_exist_process = false;
                    int queue_id = -1;
                    for(int i = 1; i < tmpMQ.size() ; i++){
                        if(!tmpMQ[i].v.empty()) {
                            lower_queue_exist_process = true;
                            queue_id = i;
                            break;
                        }
                    }
                    if(lower_queue_exist_process && tmpMQ[queue_id].previous != nullptr) {
                        #ifdef DEBUG
                        cout << "[DEBUG] Setting Queue " << queue_id << "'s process " << tmpMQ[queue_id].previous->ID << " to true!" << endl;
                        #endif
                        tmpMQ[queue_id].previous->Preempt = true;
                    }
                }
            }
        }
        // Update all JustPreempted flag
        for(auto it: tmpMQ){
            for(int i = 0 ; i < it.v.size() ; i++){
                it.v[i]->JustPreempted = false;
            }
            pq.push(it);
        }
        tmpMQ.clear();

        // Update highest priority queue's process's status
        Process* updated_process;
        MultilevelQueue updated_queue = pq.top();
        while(!pq.empty()){
            tmpMQ.push_back(pq.top());
            pq.pop();
        }
        for(int i = 0 ; i < tmpMQ.size() ; i++){
            if(!tmpMQ[i].v.empty()){
                updated_process = tmpMQ[i].update();
                updated_queue = tmpMQ[i];
                tmpMQ[i].previous = updated_process;
                break;
            }
        }
        
        // Update all other queue's status and set they are not working
        for(int i = 0 ; i < tmpMQ.size() ; i++){
            if(tmpMQ[i].order != updated_queue.order){
                tmpMQ[i].previous = nullptr;
                tmpMQ[i].updateWaitTime();
            }
        }

        // Start to handle preempt flags
        for(int i = 0 ; i < tmpMQ.size() ;i++){
            for(int j = 0 ; j < tmpMQ[i].v.size() ; j++){
                if(!tmpMQ[i].v[j]->Preempt) continue;
                Process* tmp = tmpMQ[i].v[j];
                tmp->Preempt = false;
                tmp->JustPreempted = true;
                auto it = std::find(tmpMQ[i].v.begin(), tmpMQ[i].v.end(), tmpMQ[i].v[j]);
                tmpMQ[i].v.erase(it);
                if(i != tmpMQ.size() - 1){ // exist other level Queue
                    tmpMQ[i + 1].v.push_back(tmp);
                }
                else{
                    tmpMQ[i].v.push_back(tmp);
                }
                if(tmpMQ[i].mode == 2 && tmpMQ[i].used_time == 0 && tmpMQ[i].previous != nullptr){
                    // prevent process in RR is preempted twice when time's up and high priority queue preempt it
                    tmpMQ[i].previous = nullptr;
                }
            }
        }

        // Not all processes has arrived, push all queues back
        if(!processes.empty()){
            for(int i = 0 ; i < tmpMQ.size() ; i++){
                pq.push(tmpMQ[i]);
            }
        }
        // All processes have arrived, delete high priority queues with no process
        else{
            bool high_priority_queue_empty_flag = true;
            for(int i = 0 ; i < tmpMQ.size() ; i++){
                if(tmpMQ[i].v.empty() && high_priority_queue_empty_flag){
                    continue;
                }
                high_priority_queue_empty_flag = false;
                pq.push(tmpMQ[i]);
            }
        }
        tmpMQ.clear();

        // Process has finished
        if(updated_process->RBT == 0){
            updated_process->CT = time + 1;
            updated_process->TAT = updated_process->CT - updated_process->AT;
            finish.push_back(updated_process);
        }

        // All multilevel queues are empty
        if(pq.empty()){
            break;
        }
        time++; // Update timer
        #ifdef DEBUG
        print_all_queue(pq);
        cout << "*******************************" << endl;
        #endif
    }   

    int total_wait_time = 0;
    int total_turnaround_time = 0;

    sort(finish.begin(), finish.end(), CompareProcessesByID);
    for(Process* p : finish){
        #ifdef DEBUG
        p->printInfo();
        #endif
        cout << p->WT << " " << p->TAT << endl;
        total_wait_time += p->WT;
        total_turnaround_time += p->TAT;
    }
    cout << total_wait_time << endl;
    cout << total_turnaround_time << endl;

    return 0;
}
