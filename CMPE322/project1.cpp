#include <iostream>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

/*
 * @field name: name of the instruction.
 * @field time: time it takes to finish the execution of the instruction.
 *
 */
struct Instruction {
public:
    string name;
    int time;
    Instruction(string name_, int time_){
        this->name = name_;
        this->time = time_;
    }
};
/*
 * @field id: id of the process, line number in the definition.txt file of the Process
 * @field name: name of the process.
 * @field code_file: file that contains the process' code file that contains the instructions.
 * @field arrival_time: arrival time of the process.
 * @field current_line: last instruction executed.
 * @field instructions: instructions of the process.
 * @field isFinished: true if all of the instructions are executed false otherwise.
 * @field priority: priority of the process.
 * @field queue_enter_time: most recent time that process enters the queue.
 * @field total_waiting_time: total waiting time of the queue.
 * @field exit_time: exit time of the process.
 */
struct Process {
public:
    int id;
    string name;
    int arrival_time;
    string code_file;
    int current_line;
    vector<Instruction> instructions;
    bool isFinished = false;
    int priority;
    int queue_enter_time;
    int total_waiting_time;
    int exit_time;


    Process (int id_, string name_, string code_file_, int arrival_time_, int priority_){
        this->id = id_;
        this->name = name_;
        this->code_file = code_file_;
        this->arrival_time = arrival_time_;
        current_line = 0;
        this->priority = priority_;
        this->queue_enter_time = 0;
        this->total_waiting_time =0;
        readCodeFile();
    }

    /*
     * This method read the code file of the process and adds its lines to instructions vector.
     */
    void readCodeFile () {
        string ins_name;
        int ins_time;
        ifstream instruction_file(code_file + ".txt");
        while (instruction_file >> ins_name >> ins_time) {
            Instruction instr(ins_name, ins_time);
            instructions.push_back(instr);
        }
        instruction_file.close();
    }
    /*
     * This method sets the current line of the process and it changes isFinished field accordingly.
     */
    void setCurrentLine(int current_line_){
        this->current_line = current_line_;
        this->isFinished = this->current_line == instructions.size();
    }
};

struct compareToArrivalTime {
    bool operator() (Process& p1, Process& p2) {
        return  p1.arrival_time > p2.arrival_time;
    }
};

struct compareToPriority {
    bool operator() (Process& p1, Process& p2) {
        if (p1.priority == p2.priority)
            return p1.arrival_time > p2.arrival_time;
        return p1.priority > p2.priority;
    }
};
/*
 * This method prints the queue at a given time.
 */
void printQueue(priority_queue<Process, vector<Process>, compareToPriority> ready_queue, int time, ofstream&  myFile) {
    priority_queue<Process, vector<Process>, compareToPriority> copy_queue = ready_queue;
    if (ready_queue.empty()){
        myFile << time <<":HEAD--TAIL"<<endl;
        return;
    }
    myFile << time <<":HEAD-";
    while (!copy_queue.empty()){
        Process current = copy_queue.top();
        myFile << current.name << "[" << current.current_line + 1 << "]" << "-";
        copy_queue.pop();
    }
    myFile <<"TAIL"<<endl;
}

int main(int argc, char *argv[]) {
    priority_queue <Process, vector<Process>, compareToArrivalTime> arrived_processes;
    priority_queue <Process, vector<Process>, compareToPriority> ready_processes;
    vector <Process> finished;
    ifstream process_file ("definition.txt");
    ofstream myFile("output.txt");
    string p_name, code_file;
    int priority, arrival_time;
    int id = 0;
    // This while reads the file line by line. Adds processes to arrived_processes priority queue.
    while (process_file >> p_name >> priority >> code_file >> arrival_time) {
        Process process(id, p_name, code_file, arrival_time, priority);
        arrived_processes.push(process);
        id++;
    }
    process_file.close();

    //time is the time of the system.
    int time = 0;
    printQueue(ready_processes,time, myFile);
    //taking the first process from the queue
    Process cpu_process = arrived_processes.top();
    arrived_processes.pop();
    time = cpu_process.arrival_time;
    //making the current cpu process is the first element of the reay queue.
    ready_processes.push(cpu_process);
    printQueue(ready_processes,time, myFile);
    ready_processes.pop();
    while (!arrived_processes.empty()) {
        // If the current process is not finished and there's still time to any process to arrive, execute the instructions of the process.
        while (!cpu_process.isFinished && time < arrived_processes.top().arrival_time) {
            Instruction current_instruction = cpu_process.instructions[cpu_process.current_line];
            time = time + current_instruction.time;
            cpu_process.setCurrentLine(cpu_process.current_line + 1);
        }
        //If the current process is finished and there's still time to any process to arrive.
        if (cpu_process.isFinished && time < arrived_processes.top().arrival_time) {
            //current process exits from the system.
            cpu_process.exit_time = time;
            finished.push_back(cpu_process);
            //if there are any ready process, take it to cpu.
            if (!ready_processes.empty()) {
                cpu_process = ready_processes.top();
                printQueue(ready_processes,time, myFile);
                ready_processes.pop();
                cpu_process.total_waiting_time += (time - cpu_process.queue_enter_time);
            }
                // If there is no process in the ready queue, take the next arrived process.
            else {
                printQueue(ready_processes,time, myFile);
                cpu_process = arrived_processes.top();
                arrived_processes.pop();
                time = cpu_process.arrival_time;
                ready_processes.push(cpu_process);
                printQueue(ready_processes,time, myFile);
                ready_processes.pop();
            }
        }
            // if the time is ahead of the time of the next arrived process.
        else {
            // take all of the process that had arrived before time and push them into ready queue.
            while (time >= arrived_processes.top().arrival_time && !arrived_processes.empty()){
                Process pushed = arrived_processes.top();
                pushed.queue_enter_time = time;
                pushed.total_waiting_time += (time - pushed.arrival_time);
                ready_processes.push(pushed);
                arrived_processes.pop();
            }
            // if the current process is finished, it exits the system.
            if (cpu_process.isFinished){
                cpu_process.exit_time = time;
                finished.push_back(cpu_process);
            }
                //if the current process is not finished, push it to ready queue.
            else{
                cpu_process.queue_enter_time = time;
                ready_processes.push(cpu_process);
            }
            printQueue(ready_processes,time,myFile);
            //take the first process from the ready queue.
            if(!ready_processes.empty()){
                cpu_process = ready_processes.top();
                ready_processes.pop();
                cpu_process.total_waiting_time += (time - cpu_process.queue_enter_time);
            }

        }

    }
    //if there are no arriving processes, take the processes from the ready queue one by one and execute them.
    while(!ready_processes.empty()){
        while (!cpu_process.isFinished){
            Instruction current_instruction = cpu_process.instructions[cpu_process.current_line];
            time = time + current_instruction.time;
            cpu_process.setCurrentLine(cpu_process.current_line + 1);
        }
        cpu_process.exit_time = time;
        finished.push_back(cpu_process);
        printQueue(ready_processes, time, myFile);
        cpu_process = ready_processes.top();
        ready_processes.pop();
        cpu_process.total_waiting_time += (time - cpu_process.queue_enter_time);
    }
    //execute the last process.
    while (!cpu_process.isFinished){
        Instruction current_instruction = cpu_process.instructions[cpu_process.current_line];
        time = time + current_instruction.time;
        cpu_process.setCurrentLine(cpu_process.current_line + 1);
    }
    cpu_process.exit_time = time;
    finished.push_back(cpu_process);
    printQueue(ready_processes,time, myFile);
    // sort processes by id.
    for (int i = 0; i < finished.size(); i++){
        for (int j = i; j < finished.size(); j++){
            if (finished[j].id < finished[i].id){
                Process tmp = finished[j];
                finished[j] = finished[i];
                finished[i] = tmp;
            }
        }
    }
    myFile << endl;
    for (int i = 0; i < finished.size(); i++){
        Process current = finished[i];
        int turnaround_time = current.exit_time - current.arrival_time;
        myFile << "Turnaround time for " << current.name << " = " << turnaround_time << " ms" << endl;
        myFile << "Waiting time for " << current.name << " = " << current.total_waiting_time << endl;
    }
    return 0;
}