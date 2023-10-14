#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>

using namespace std;

class Process {
public:
    int AT; // Arrival time
    int BT; // Burst time
    int RBT; // Remaining burst time
    int CT; // Completion time
    int TAT; // Turnaround time
    int WT; // Waiting time
    int id; // Process ID
};

bool compareID(const Process& p1, const Process& p2) {
    return p1.id < p2.id;
}

// Function to execute processes in Round Robin (RR)
void execute_rr(queue<Process>& queue, int time_quantum, int current_time) {
    if (queue.empty()) return;

    Process& process = queue.front();
    int remaining_time = process.RBT;
    if (remaining_time <= time_quantum) {
        // Process completes within the time quantum
        process.CT = current_time + remaining_time;
        process.TAT = process.CT - process.AT;
        process.WT = process.TAT - process.BT;
        process.RBT = 0; // Process is done
        queue.pop();
    } else {
        // Process doesn't complete within the time quantum
        process.CT = current_time + time_quantum;
        process.RBT -= time_quantum;
        queue.pop();
    }
}

// Function to execute processes in Shortest Remaining Time First (SRTF)
void execute_srtf(queue<Process>& queue, int current_time) {
    if (queue.empty()) return;

    Process& shortest = queue.front();
    for (const Process& process : queue) {
        if (process.RBT < shortest.RBT && process.AT <= current_time) {
            shortest = process;
        }
    }

    if (shortest.RBT > 0) {
        shortest.CT = current_time + 1;
        shortest.RBT--;
        if (shortest.RBT == 0) {
            shortest.TAT = shortest.CT - shortest.AT;
            shortest.WT = shortest.TAT - shortest.BT;
            queue.pop();
        }
    }
}

// Function to execute processes in First-Come-First-Serve (FCFS)
void execute_fcfs(queue<Process>& queue, int current_time) {
    if (queue.empty()) return;

    Process& process = queue.front();
    if (process.AT <= current_time) {
        process.CT = current_time + process.BT;
        process.TAT = process.CT - process.AT;
        process.WT = process.TAT - process.BT;
        queue.pop();
    }
}

int main() {
    int num_queues, num_processes;
    cin >> num_queues >> num_processes;

    vector<queue<Process>> queues(num_queues);
    vector<int> time_quantum(num_queues);

    for (int i = 0; i < num_queues; ++i) {
        int mode;
        cin >> mode;
        if (mode == 2) {
            cin >> time_quantum[i];
        } else {
            time_quantum[i] = -1;
        }
    }

    for (int i = 0; i < num_processes; ++i) {
        int arrival_time, burst_time;
        cin >> arrival_time >> burst_time;
        Process process;
        process.AT = arrival_time;
        process.BT = burst_time;
        process.RBT = burst_time;
        process.id = i;
        // Assign processes to the appropriate queues based on input order
        queues[i % num_queues].push(process);
    }

    int current_time = 0;

    while (true) {
        // Add your queue execution logic here
        for (int i = 0; i < num_queues; ++i) {
            if (i == 0) {
                execute_srtf(queues[0], current_time);
            } else if (i == 1) {
                execute_rr(queues[1], time_quantum[1], current_time);
            } else if (i == 2) {
                execute_fcfs(queues[2], current_time);
            }
        }

        bool allQueuesEmpty = true;
        for (const auto& queue : queues) {
            if (!queue.empty()) {
                allQueuesEmpty = false;
                break;
            }
        }

        if (allQueuesEmpty) {
            break;
        }

        current_time++;
    }

    // Calculate and print turnaround time and waiting time for each process
    vector<Process> allProcesses;
    for (auto& queue : queues) {
        while (!queue.empty()) {
            allProcesses.push_back(queue.front());
            queue.pop();
        }
    }

    sort(allProcesses.begin(), allProcesses.end(), compareID);

    for (const Process& process : allProcesses) {
        cout << process.WT << " " << process.TAT << endl;
    }

    int total_wait_time = 0;
    int total_turnaround_time = 0;
    for (const Process& process : allProcesses) {
        total_wait_time += process.WT;
        total_turnaround_time += process.TAT;
    }

    cout << total_wait_time << endl;
    cout << total_turnaround_time << endl;

    return 0;
}
