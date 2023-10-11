#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

struct Process {
    int arrival_time;
    int burst_time;
    int remaining_time;
    int wait_time;
    int turnaround_time;
};

// Function for FCFS scheduling
void fcfs(vector<Process>& processes) {
    int current_time = 0;
    int total_wait_time = 0;
    int total_turnaround_time = 0;

    for (int i = 0; i < processes.size(); i++) {
        if (processes[i].arrival_time <= current_time) {
            processes[i].wait_time = current_time - processes[i].arrival_time;
            processes[i].turnaround_time = processes[i].wait_time + processes[i].burst_time;
            total_wait_time += processes[i].wait_time;
            total_turnaround_time += processes[i].turnaround_time;
            current_time += processes[i].burst_time;
        } else {
            processes[i].wait_time = 0;
            processes[i].turnaround_time = processes[i].burst_time;
            total_turnaround_time += processes[i].burst_time;
            current_time = processes[i].arrival_time + processes[i].burst_time;
        }
    }

    for (int i = 0; i < processes.size(); i++) {
        cout << processes[i].wait_time << " " << processes[i].turnaround_time << endl;
    }

    cout << total_wait_time << endl;
    cout << total_turnaround_time << endl;
}

// Function for SRTF scheduling
void srtf(vector<Process>& processes) {
    int current_time = 0;
    int completed_processes = 0;
    int total_wait_time = 0;
    int total_turnaround_time = 0;

    while (completed_processes < processes.size()) {
        int shortest_remaining_time = INT_MAX;
        int shortest_index = -1;
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time < shortest_remaining_time && processes[i].remaining_time > 0) {
                shortest_remaining_time = processes[i].remaining_time;
                shortest_index = i;
            }
        }
        if (shortest_index == -1) {
            current_time++;
        } else {
            processes[shortest_index].remaining_time--;
            current_time++;
            if (processes[shortest_index].remaining_time == 0) {
                processes[shortest_index].wait_time = current_time - processes[shortest_index].arrival_time - processes[shortest_index].burst_time;
                processes[shortest_index].turnaround_time = processes[shortest_index].wait_time + processes[shortest_index].burst_time;
                total_wait_time += processes[shortest_index].wait_time;
                total_turnaround_time += processes[shortest_index].turnaround_time;
                completed_processes++;
            }
        }
    }

    for (int i = 0; i < processes.size(); i++) {
        cout << processes[i].wait_time << " " << processes[i].turnaround_time << endl;
    }

    cout << total_wait_time << endl;
    cout << total_turnaround_time << endl;
}

int main() {
    int N, M;
    cin >> N >> M;

    vector<int> mode(N), time_quantum(N);
    for (int i = 0; i < N; i++) {
        cin >> mode[i] >> time_quantum[i];
    }

    vector<Process> processes(M);
    for (int i = 0; i < M; i++) {
        cin >> processes[i].arrival_time >> processes[i].burst_time;
        processes[i].remaining_time = processes[i].burst_time;
    }

    for (int i = 0; i < N; i++) {
        if (mode[i] == 0) {
            fcfs(processes);
        } else if (mode[i] == 1) {
            srtf(processes);
        }
    }

    return 0;
}
