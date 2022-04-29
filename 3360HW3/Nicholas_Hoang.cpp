#include <sys/types.h>
#include<sys/sem.h>
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
void PRP(){

}
void HDD(){

}
void PFH(){

}
int main(int argc, char** argv){
    //input stream to read inputs
    //Parameters
    std::ifstream file;
    //process precedence graph
    std::vector<std::vector<int> > graph;
    //words
    std::vector<std::string> words;

    file.open(argv[1]);
    //check if file is open
    if(file.is_open()){
        //input into "graph"
        std::string input = "";
        while (getline(file, input)) {
             std::istringstream ss(input);
             std::vector<int> row;
             int help;
             while (ss >> help) {
                row.push_back(help);
             }
            graph.push_back(row);
         }
    }else{
        //failsafe
        fprintf(stderr, "invalid arguments");
        exit(1);
    }
    //close file 1
    file.close();
    //read words
    file.open(argv[2]);
    if(file.is_open()){
        std::string input = "";
        while(getline(file, input, ',')){
            if(input[0] == ' ')
            {
                input = input.substr(1, input.length()-1);
            }
            words.push_back(input);
        }
    }else{
        exit(1);
    }
    file.close();
    return 0;
}
