#include <sys/types.h>
#include <sys/wait.h>
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

void printParse(std::string input, std::vector<std::string> words){
    std::unordered_map<int, std::string> english;
    english[0] = "";
    english[1] = "one";
    english[2] = "two";
    english[3] = "three";
    english[4] = "four";
    english[5] = "five";
    english[6] = "six";
    english[7] = "seven";
    english[8] = "eight";
    english[9] = "nine";
    english[10] = "ten";
    english[11] = "eleven";
    english[12] = "twelve";
    english[13] = "thirteen";
    english[14] = "fourteen";
    english[15] = "fifteen";
    english[16] = "sixteen";
    english[17] = "seventeen";
    english[18] = "eighteen";
    english[19] = "nineteen";
    english[20] = "twenty";
    english[21] = "twenty one";
    english[22] = "twenty two";
    english[23] = "twenty three";
    english[24] = "twenty four";
    english[25] = "twenty five";
    english[26] = "twenty six";
    english[27] = "twenty seven";
    english[28] = "twenty eight";
    english[29] = "twenty nine";
    english[30] = "thirty";
    english[40] = "forty";
    english[50] = "fifty";
    english[60] = "sixty";
    english[70] = "seventy";
    english[80] = "eighty";
    english[90] = "ninety";
    english[100] = "hundred";
    english[1000] = "thousand";

    int array[words.size()];
    for(int i = 0; i < words.size(); i++){
        array[i] = 0;
    }
    for(int i = 0; i < words.size(); i++){
        std::string temp = input;
        int ind = 0;
        while((ind =temp.find(words.at(i))) != std::string::npos){
            temp = temp.substr(ind+words.at(i).length(),temp.length()-ind);
            array[i] += 1;
        }
    }
    for(int i = 0; i < words.size(); i++){
        if(array[i] == 1){
            std::cout << words.at(i);
        }else{
            if(array[i]/10 > 0){
                std::cout << english[(array[i]/10)*10];
                if(array[i]%10 != 0){
                    std::cout << " ";
                }
                
            }
            std::cout << english[array[i]%10] <<" " << words.at(i) << "s";
        }
        if(i != words.size()-1){
            std::cout << ", ";
        }else{
            std::cout <<"\n";
        }
    }
}
struct vert {
    int vID;
    bool isInput;
    bool isOut;
    std::vector<std::string> words;
    std::vector<int> inData;
    std::vector<int> outData;
    std::vector<int> inPipes;
    std::vector<int> outPipes;
    vert(){
        vID = -1;
        isInput = false;
        isOut = false;
    }
    vert(std::vector<std::vector<int> > ar, int num,bool inn, bool oo){
        vID = num;
        isInput = inn;
        isOut = oo;
        for(int r = 0; r < ar.size(); r++){
            if(ar.at(num).at(r) == 1){
                outData.push_back(r);
            }
        }
        for(int c = 0; c < ar.size(); c++){
            if(ar.at(c).at(num) == 1){
                inData.push_back(c);
            }
        }
    }
    void setIO(std::vector<std::vector<int> > ar, int num){
        vID = num;
        for(int r = 0; r < ar.size(); r++){
            if(ar.at(num).at(r) == 1){
                outData.push_back(r);
            }
        }
        for(int c = 0; c < ar.size(); c++){
            if(ar.at(c).at(num) == 1){
                inData.push_back(c);
            }
        }
    }
    void print(){
        std::cout << "v" << vID << ": " << "\n";
        std::cout << "reads from:" << "\n";
        for(int i = 0; i < inData.size(); i++){
            std::cout << "-v" << inData.at(i) << "\n";
        }
        std::cout << "and writes to:" << "\n";
        for(int i = 0; i < outData.size(); i++){
            std::cout << "-v" << outData.at(i) << "\n";
        }
        std::cout << "InPipes: " << std::endl;
        for(int i = 0; i < inPipes.size(); i++){
            std::cout << inPipes.at(i) << " , "; 
        }
        std::cout << "\n";
        std::cout << "OutPipes: " << std::endl;
        for(int i = 0; i < outPipes.size(); i++){
            std::cout << outPipes.at(i) << " , "; 
        }
        std::cout << "\n" << std::endl;
    }
};
//count amount of connections
int count_(std::vector<std::vector<int> > ar){
    int a = 0;
    for(int i = 0; i < ar.size(); i++){
        int isEmpty = true;
        for(int j = 0; j < ar.size(); j++){
            if(ar.at(i).at(j)){
                a++;
                isEmpty = false;
            }
        }
        if(isEmpty){
            a++;
        }
    }
    return a;
}

//checks if the vertice in question is an input
bool is_inVert(std::vector<std::vector<int> > ar,int col){
    bool isVert = true;
    //checks to see if vert has any incoming connections
    for(int c = 0; c < ar.size(); c++){
        if(ar.at(c).at(col) == 1){
            isVert = false;
        }
    }
    return isVert;
}
//check if vertex is output
bool is_outVert(std::vector<std::vector<int> > ar,int col){
    bool isVert = true;
    //checks to see if vert has any incoming connections
    for(int c = 0; c < ar.size(); c++){
        if(ar.at(col).at(c) == 1){
            isVert = false;
        }
    }
    return isVert;
}

//count amount of vertices that are inputs
int countInvert(std::vector<std::vector<int> > ar,int k){
    int a = 0;
    for(int i = 0; i < k; i++){
        if(is_inVert(ar,i)){
            a++;
        }
    }
    return a;
}
//MAIN CALCULATION
void process(std::vector<std::vector<int> > ar, std::vector<std::string> words, int k){
    std::vector<vert> verts;

    //Initialize all vertices and their connections
    for(int i = 0; i < k; i++){
        verts.push_back(vert());
    }
    int ss = words.size()-1;
    for(int i = k-1; i >= 0; i--){
        //Initialize all vertices
        if(i == k){
            //the last vertex to pipe to
            verts.at(i) = (vert());
        }
        else{
            //all other vertices
            vert vTemp = vert(ar,i,is_inVert(ar,i),is_outVert(ar,i));
            //if it's an input, give it the initial word
            if(vTemp.isInput){
                vTemp.words.push_back(words.at(ss));
                ss--;
            }
            // if(vTemp.isOut){
            //     vTemp.outData.push_back(k);
            // }
            verts.at(i) = (vTemp);
        }
    }
    // initialize pipes
    int pipes[count_(ar)][2];
    for (int p = 0; p < count_(ar); p++) {
        if (pipe(pipes[p]) == -1) {
            fprintf(stderr, "%i pipe failed", p);
            exit(1);
        }
    }
    int at_pipe = 0;
    for (int v = 0; v < verts.size(); v++) {
        for (int j = 0; j < verts[v].outData.size(); j++) {
            verts[v].outPipes.push_back(at_pipe);
            at_pipe++;
        } 
    }
    // set read_from pipes for all vertices
    for (int v = 0; v < verts.size(); v++) {
        int wto = 0;
        for (int j = 0; j < verts[v].outPipes.size() && wto < verts[v].outData.size(); j++, wto++) {
            verts[verts[v].outData[wto]].inPipes.push_back(verts[v].outPipes[j]);
        }
    }
    // set write_to pipes for output vertices
    for (int v = 0; v < verts.size(); v++) {
        if (verts[v].isOut) {
            verts[v].outPipes.push_back(at_pipe);
            at_pipe++;
        }
    }
    // # of forks depends on size of matrix: k
    char charTemp[100];
    int pnum = -1, pid;
    static int pcount = 0;
    for (int f = 0; f < k; f++) {
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "%i fork failed", f);
            exit(1);
        }
        else if (pid == 0) {
            pnum = f;
            break;
        }
    }

    if(pnum != -1){
        
        char cAr[256];
        int cArIndex = 0;
        for(int z = 0; z < 256; z++){
            cAr[z] = '\0';
        }
        //std::cout << "PNUM: " << pnum << std::endl;
        //input vertex
        if(is_inVert(ar,pnum)){
            //WRITING FOR INPUT VERTICES
            for(int j = 0; j < verts.at(pnum).outPipes.size(); j++){

                //close read end
                close(pipes[verts.at(pnum).outPipes.at(j)][0]);
                //write out to write end of pipe
                write(pipes[verts.at(pnum).outPipes.at(j)][1],verts.at(pnum).words.at(0).c_str(),strlen(charTemp)+1);
                close(pipes[verts.at(pnum).outPipes.at(j)][1]);
                //std::cout << "WRITE: v" << pnum << " -> v" << verts.at(pnum).outData.at(j) << ": " << verts.at(pnum).words.at(0).c_str() << " at pipe #" << verts.at(pnum).outPipes.at(j) << std::endl;
                //close write end
                
            }
            return;
        }
        //If it is an internal vertex
        else{
            //READ FROM PIPE
            
            for(int j = 0; j < verts.at(pnum).inPipes.size(); j++){
                //close writing end of pipe
                close(pipes[verts.at(pnum).inPipes.at(j)][1]);
                //read in values of pipe
                read(pipes[verts.at(pnum).inPipes.at(j)][0], charTemp, 256);
                close(pipes[verts.at(pnum).inPipes.at(j)][0]);
                //std::cout << "READ: v" << verts.at(pnum).inPipes.at(j) << " -> v" << pnum << ": " << charTemp << " at pipe# " << verts.at(pnum).inPipes.at(j) << "\n";
                for (int kk = 0; kk < strlen(charTemp); kk++){
                    if (isalpha(charTemp[kk])){
                        cAr[cArIndex++] = charTemp[kk];
                    } 
                }
            }

            //WRITE TO PIPE
            for(int j = 0; j < verts.at(pnum).outPipes.size(); j++){

                //close read end
                close(pipes[verts.at(pnum).outPipes.at(j)][0]);
                //write out to write end of pipe
                write(pipes[verts.at(pnum).outPipes.at(j)][1],cAr,strlen(cAr)+1);
                //debug
                //std::cout << "WRITE: v" << pnum << " -> v" << verts.at(pnum).outData.at(j) << ": " << verts.at(pnum).words.at(0).c_str() << " at pipe #" << verts.at(pnum).outPipes.at(j) << std::endl;
                //close write end
                close(pipes[verts.at(pnum).outPipes.at(j)][1]);
            }
            return;
        }
    }else{
        //wait until children finish
        while(wait(NULL) > 0);
        char output[256];
        int oIndex = 0;
        for(int z = 0; z < 256; z++){
            output[z] = '\0';
        }
        for(int r = 0; r < verts.size(); r++){
            //if it is an output 
            if(verts.at(r).isOut){
                for(int c = 0; c < verts.at(r).outPipes.size(); c++){
                    close(pipes[verts.at(r).outPipes.at(c)][1]);
                    read(pipes[verts.at(r).outPipes.at(c)][0], charTemp, 256);
                    close(pipes[verts.at(r).outPipes.at(c)][0]);
                    for (int kk = 0; kk < strlen(charTemp); kk++){
                         if (isalpha(charTemp[kk])){
                             output[oIndex] = charTemp[kk];
                             oIndex++;
                         } 
                    }
                }
            }
        }
        std::string res(output);
        printParse(res,words);
        return;
    }
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
    process(graph,words,graph.size());
    return 0;
}
