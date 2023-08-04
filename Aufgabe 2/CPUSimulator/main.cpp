#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>

static int pid = 0;
const int waitTime = 4;
int runningSimulation = 1;
int quantumCounter = 0;
int quantum = 4;

enum class status {
    running = 0, ready = 1, blocked = 2
};

struct Process {
    int start;
    int end;
    int processId;
    std::string name;
    std::vector<std::string> VectorInstruction;
    int index;
    int data;
    int tmp_acc;
    status processStatus;
    int wait;
    bool waitEnough;

    Process() {
        processId = pid;
        pid++;
        data = 0;
        index = 0;
        processStatus = status::ready;
        wait = 0;
        waitEnough = false;
    }
};

struct Scheduler {
    std::vector<Process> readyProcesses;
    std::vector<Process> waitingProcesses;
    std::vector<Process> doneProcesses;

    bool getReadyProcess() {
        if (readyProcesses.size() > 0) return true;
        return false;
    }

    int updateWait() {
        int blockedPID = -1;
        if (waitingProcesses.size() > 0) {
            if (waitingProcesses.size() == 1){
                if(waitingProcesses.at(0).wait != 0){
                    waitingProcesses.at(0).wait--;
                    blockedPID = waitingProcesses.at(0).processId;
                }
            }
            else{
                for (unsigned int i = 0; i < waitingProcesses.size()-1; i++) {
                    if (waitingProcesses.at(i).wait == 0){
                        if (waitingProcesses.at(i+1).wait != 0){
                            waitingProcesses.at(i+1).wait--;
                            blockedPID = waitingProcesses.at(i+1).processId;
                        }
                    }
                    else {
                        waitingProcesses.at(i).wait--;
                        blockedPID = waitingProcesses.at(i).processId;
                    }
                }
            }
            for (unsigned int i = 0; i < waitingProcesses.size(); i++) {
                if (waitingProcesses.at(i).wait == 0 && waitingProcesses.at(i).waitEnough) {
                    waitingProcesses.at(i).waitEnough = false;
                    waitingProcesses.at(i).processStatus = status::ready;
                    if(!getReadyProcess()) waitingProcesses.at(i).processStatus = status::running;
                    readyProcesses.push_back(waitingProcesses.at(i));
                    waitingProcesses.erase(waitingProcesses.begin() + i);

                }else if (waitingProcesses.at(i).wait == 0 && !waitingProcesses.at(i).waitEnough){
                    waitingProcesses.at(i).waitEnough = true;
                    blockedPID = waitingProcesses.at(i).processId;
                }
            }
        }
        return blockedPID;
    }

    void blockProcess(Process process) {
        if (process.processStatus == status::blocked) return;
        process.processStatus = status::blocked;
        process.wait = waitTime;
        waitingProcesses.push_back(process);
        for (unsigned int i = 0; i < readyProcesses.size(); i++) {
            if (process.processId == readyProcesses.at(i).processId)
                readyProcesses.erase(readyProcesses.begin() + i);
        }
    }

    Process& getRunningProcess() {
        for (Process& p : readyProcesses) {
            if (p.processStatus == status::running) return p;
        }
    }

    void switchProcess() {
        readyProcesses.at(0).processStatus = status::running;
    }

    void destroyProcess() {
        for (unsigned int i = 0; i < readyProcesses.size(); i++) {
            if (readyProcesses.at(i).processStatus == status::running) {
                readyProcesses.erase(readyProcesses.begin() + i);
            }
        }
        if (getReadyProcess()) switchProcess();
        else if (waitingProcesses.size() == 0) runningSimulation = 0;
    }
};

struct Cpu {
    int takt = 0;
    int pc = 0;
    int acc = 0;
    int blockedPID;

    void init(Scheduler& scheduler) {
        createProcess(scheduler, "init");
        execute(scheduler, scheduler.readyProcesses.front());
    }

    void createProcess(Scheduler& scheduler, std::string program) {
        std::string path = program;
        std::string opcode, variable;
        std::ifstream infile(path);
        Process newProcess;

        newProcess.name = program;
        newProcess.index = 0;
        newProcess.processStatus = status::running;
        newProcess.start = takt+1;

        //Read File
        while (infile >> opcode) {
            if (opcode != "P" && opcode != "Z") {
                infile >> variable; newProcess.VectorInstruction.push_back(opcode + " " + variable);
            }
            else {
                variable = " ";
                newProcess.VectorInstruction.push_back(opcode + variable);
            }
        }
        scheduler.readyProcesses.push_back(newProcess);
    }

    void execute(Scheduler& scheduler, Process process) {
        while (runningSimulation == 1) {
            blockedPID = scheduler.updateWait();
            if (scheduler.getReadyProcess()) {
                quantumCounter = 0;
                process = scheduler.getRunningProcess();
                pc = process.index;
                acc = process.data;
                takt++;
            }
            else {
                takt++;
                //log_p
                std::cout << std::setw(10) << takt << std::setw(10) << "-" << std::setw(10) << "----"
                          << std::setw(10) << pc  << std::setw(10) << acc << std::setw(10);
                if (blockedPID==-1) std::cout << "-";
                else std::cout << blockedPID;
                std::cout << std::setw(10) << std::setw(10)<< "----" << std::endl;

                continue;
            }

            while (pc < process.VectorInstruction.size()) {
                quantumCounter++;
                std::string command = process.VectorInstruction.at(pc);
                //log
                std::cout << std::setw(10) << takt << std::setw(10) << process.processId << std::setw(10) << process.name
                          << std::setw(10) << pc << std::setw(10) << acc << std::setw(10);
                if (blockedPID==-1) std::cout << "-";
                else std::cout << blockedPID;
                std::cout << std::setw(10)<< command << std::endl;


                std::string opcode, variable;
                std::string delimiter = " ";

                size_t pos = command.find(delimiter);
                opcode = command.substr(0, command.find(delimiter));
                variable = command.substr(pos + 1);

                if(quantumCounter%quantum == 0 ){
                    opcode = "P";

                    pc-=1;
                }
                //LOAD
                if (opcode.at(0) == 'L') {
                    acc = std::stoi(variable);
                }
                //ADD
                else if (opcode.at(0) == 'A') {
                    acc += std::stoi(variable);
                }
                //SUB
                else if (opcode.at(0) == 'S') {
                    acc -= std::stoi(variable);
                }
                //Peripheral
                else if (opcode.at(0) == 'P') {
                    process.index = pc + 1;
                    process.data = acc;
                    scheduler.blockProcess(process);
                    if (scheduler.getReadyProcess()) {
                        scheduler.switchProcess();
                        break;
                    }
                    else break;

                }
                //Execute
                else if (opcode.at(0) == 'X') {

                    scheduler.getRunningProcess().index = pc + 1;
                    scheduler.getRunningProcess().data = acc;
                    scheduler.getRunningProcess().processStatus = status::ready;

                createProcess(scheduler, variable);
                break;
                }
                //End Programm
                else if (opcode.at(0) == 'Z') {
                    process.tmp_acc = acc;
                    scheduler.destroyProcess();
                    process.end = takt;
                    scheduler.doneProcesses.push_back(process);
                    break;
                }
                pc++;
                takt++;
                scheduler.updateWait();
            }
        }
    }
};

int main() {

    std::cout << std::setw(10) << "Takt " << std::setw(10) << "PID " << std::setw(10) << "Process "
              << std::setw(10) << "PC " << std::setw(10) << "Acc " << std::setw(10) << "E/A"<< std::setw(10) << "Befehl" << std::endl;
    Cpu cpu;
    Scheduler scheduler;
    cpu.init(scheduler);

    std::cout << "Zusammenfassung der Simulation: " << std::endl;

    std::cout << '\n';

    std::cout << "Simulierte Takte: " << cpu.takt << std::endl;

    for (unsigned int i = 0; i < scheduler.doneProcesses.size(); i++) {
         std::cout << scheduler.doneProcesses[i].name << std::endl;
         std::cout << "Start: " << scheduler.doneProcesses[i].start << std::endl;
         std::cout << "End: " << scheduler.doneProcesses[i].end << std::endl;
         std::cout << "VerweilZeit: " << scheduler.doneProcesses[i].end - scheduler.doneProcesses[i].start +1 << std::endl;
         std::cout << "ACC: " << scheduler.doneProcesses[i].tmp_acc << std::endl;
         std::cout << '\n';
    }

    std::cout << "Simulation end" << std::endl;
    return 0;
}
