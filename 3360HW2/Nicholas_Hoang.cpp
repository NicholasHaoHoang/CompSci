#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include<sstream>
#include<fstream>
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include <sys/mman.h>

struct process{
    int id, dl, ct;
    bool done = false;
    std::string ms = "";
    std::vector<std::string> queue;
    std::vector<std::string> instructions;
    process(int inId, int inDl, int inCt, std::vector<std::string> inIns){
        id = inId;
        dl = inDl;
        ct = inCt;
        instructions = inIns;
    }
    void print(){
        std::cout << "ID: " << id << " deadline: " << dl << " computation time: " << ct << "\n";
        for(int i = 0; i < instructions.size(); i++){
            std:: cout << instructions[i] << "\n";
        }
        std::cout << std::endl;
    }
    //calculate method
    void calculate(int x){
        ct -= x;
        //printAlc(*this);
    }
    //use resouces method
    void use_resources(int x){
        ct -= x;
    }
    //print resources used function
    void print_resources_used(int pid){
        ct -= 1;
        std::cout << "Process ID: " << pid << "\nMaster String: " << ms;
    }


};
//ENDOF: process struct

struct resource{
    char resourceId;
    int maxAvailable;
    int currAvailable;
    void setResourceId(char resourceId)
    {
        this->resourceId = resourceId;
    }
    void setCurrentAvailable(int currentAvailable)
    {
        this->currAvailable = currentAvailable;
    }
    void setMaxAvailable(int maxAvailable)
    {
        this->maxAvailable = maxAvailable;
    }
};
//ENDOF: resource struct


//TODO: Finish Output Method
void printAlc(process p){
    //Print Arrays: available, allocation, need
    //Print Deadline misses (int)
    //std::cout << "Allocation: " << alc << "\nNeed: " << need << "\nDeadline Misses: " << dlm << std::endl;
}
std::vector<int> closestDeadline(std::vector<process> plist){
    int min = plist.at(0).dl;
    std::vector<int> minP;
    minP.push_back(0);
    for(int i = 1; i < plist.size(); i++){
        if(plist.at(i).dl == min){
            minP.push_back(i);
        } else if(plist.at(i).dl < min){
            minP.clear();
            minP.push_back(i);
            min = plist.at(i).dl;
        }
    }
    return minP;
}

//TODO: Complete Longest Job Calculation
int ljf(std::vector<process> plist){

    return 0;
}

//request function
void request(){

}


//MAIN
int main(int argc, char** argv){
    //input stream to read inputs
    //Parameters
    std::ifstream file;
    //integers m and n
    int m,n;
    //available
    std::vector<int> available;
    //max 2d array
    std::vector<std::vector<int> > max;
    //process list
    std::vector<process> plist;
    

    file.open(argv[1]);
    //check if file is open
    std::string input;
    if(file.is_open()){
        //read m and n
        //m = num of resources
        //n = num of processes
        file >> m >> n;
        std::cout << "m: " << m << " n:" << n;
        getline(file,input);
        getline(file,input);
        std::stringstream a(input);
        int helper;
        while(a >> helper){
            available.push_back(helper);
        }
        for(int i = 0; i < n; i++){
            std::vector<int> temp;
            std::stringstream a(input);
            int helper;
            while(a >> helper){
                available.push_back(helper);
            }
            max.push_back(temp);
        }
    //Reading and setting processes and the commands
    while (!file.eof()) {
        getline(file, input); // current input should be process_n or newline
        if (input.substr(0, 8) == "process_") {
            int id, deadline, comp_time;
            std::vector<std::string> instructions;
            id = stoi(input.substr(8, input.length() - 1));
            getline(file, input); // deadline_n
            deadline = stoi(input);
            getline(file, input); // computation_time_n
            comp_time = stoi(input);
            // current input should be comp_time_n
            while (getline(file, input)) { // first instruction after comp_time_n line
                instructions.push_back(input);
                if (input.substr(0, 4) == "end.") break;
            }
            process p(id, deadline, comp_time, instructions);
            plist.push_back(p);
        }
    }
    file.close();
    }else{
        //failsafe
        fprintf(stderr, "invalid arguments");
        exit(1);
    }
    //close file 1
    file.close();
    for(int i = 0; i < plist.size(); i++){
        plist[i].print();
    }
    






    //Begin Processing
    //create semaphores
    //semaphore id
    int sid;
    sid = semget(123,plist.size(),IPC_CREAT | 0666); //declaring the semaphore 
    //1st is the id 
    //2nd is number of semaphores made
    //3rd is a flag
    struct sembuf sb[2]; // create sembuf to control
    sb[0].sem_num = 0; // select semaphore 0
    sb[0].sem_op = -1;//decrement semaphore by 1 (wait) sb[0] will be our wait operation
    sb[0].sem_flg = 0;

    sb[1].sem_num = 0;
    sb[1].sem_op =  1;//increment semaphore by 1 (release) sb[1] will be our release operation
    sb[1].sem_flg = 0;

    semop(sid,&sb[1],1); // default value of a semaphore is 0 (waiting for a release) so you have to increment it by one like here
                         // in order for the first process that hits semaphore 0 to keep running.


    //shared memory
    std::vector<std::string>* resources = new std::vector<std::string>[m]; //resources shared variable
    int* gt; //global time variable
    int* runningProcesses; //current running processes variable
    int* processSync;
    int tE = 0; //expected variable, doesn't need to be shared
    //shared memory ID's
    int rpID = shmget(321,sizeof(gt),IPC_CREAT | 0666);
    int gtID = shmget(321,sizeof(runningProcesses),IPC_CREAT | 0666);
    int resourceID = shmget(666,sizeof(resources), IPC_CREAT | 0666);

    //create shared memory
    
    gt = (int*)mmap(NULL, sizeof *gt, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    runningProcesses = (int*)mmap(NULL, sizeof *runningProcesses, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    processSync = (int*)mmap(NULL, sizeof *processSync, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    
    //Make sure shared memory ID was successful
    if(resourceID<0 || gtID < 0 || rpID < 0){
        std::cout<<"error with shared memory ID"<<std::endl;
        exit(1);
    }
    //fill in shared memory
    *runningProcesses = n;
    *gt = 0;
    *processSync = 0;

    //fork
    int pnum = -1, pid;
    for(int k =0;k<plist.size();k++){
    pid = fork();
        if(pid == 0){
        pnum = k;
        break;
        }else if(pid < 0){
            fprintf(stderr, "%i fork failed", k);
            exit(1);
        }
    }
    if (pnum == -1){ //Parent process
        while(*runningProcesses > 0){
            //Used to sync processes
            //sb[0].sem_num = 0 is the parent process, so start from 1
            for(int p = 1; p < plist.size(); p++){
                //wait for all processes to finish first
                //set to p
                sb[0].sem_num = p;
                sb[1].sem_num = p;
                semop(sid, &sb[0], 1);
                //increment global time variable when parent function reaches this point
                *gt += 1;
            }
        }

    }
    else{
        for(int i = 0; i < plist.at(pnum).instructions.size(); i++){
            std::string curCommand = plist.at(pnum).instructions.at(0);
            plist.at(pnum).instructions.erase(plist.at(pnum).instructions.begin());
            if(*gt < tE) i--;
            if(curCommand.substr(0,3) == "end"){
                //terminate
                *runningProcesses--;
                break;
            }
            else if(curCommand.substr(0,8) == "release("){
                //reallocate m integers to available
                

            }
            else if(curCommand.substr(0,8) == "request("){
                //allocate m integers to be used in use_resources
                for(int j = 0; j < curCommand.length();j++){
                    if(curCommand.at(j) == ',') curCommand.at(j) = ' ';
                    std::vector<int> parameters;
                    std::stringstream ss(curCommand);
                    int incr;
                    while (ss >> incr){
                        parameters.push_back(incr);
                    }
                    tE += 1;
                    
                }
            }
            else if(curCommand.substr(0,10) == "calculate("){
                //deduct calculation time without using resources
                plist.at(pnum).calculate(std::stoi(curCommand.substr(10,curCommand.length()-1)));
                
            }
            else if(curCommand.substr(0,14) == "use_resources("){
                //allocate m integers to be used in use_resources
                
            }
            else if (curCommand.find("print_resources_used") == 0) { // uses 1 comp_time
                    plist.at(i).print_resources_used(pid);
                    tE += 1;
                }
        }
    }
    sleep(1);
    munmap(gt, sizeof *gt);
    munmap(runningProcesses, sizeof *runningProcesses);
    munmap(processSync, sizeof *processSync);
    shmctl(rpID,0,IPC_RMID);
    shmctl(gtID,0,IPC_RMID);
    shmctl(resourceID,0,IPC_RMID);
    semctl(sid,0,IPC_RMID);
        








    //END
    return 0;

}
