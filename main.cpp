/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: dede
 *
 * Created on April 3, 2017, 11:33 AM
 */

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <vector>
using namespace std;

string makeDir = "/usr/bin/make";
string workDir = "temp";
string makefileDir = "makefile";
string compiler = "MyCC=g++";
string fileName = "Filename=test";
string extendName = "ExtendName=.cpp";
vector<string> filterVec;
enum MakeCMD{Make_Compile, Make_PreCompile, Make_EXE, Make_Clean};
bool make(MakeCMD cmd)
{
    pid_t pid = fork();
    if(pid == -1)
        return false;
    string cmdStr;
    switch(cmd)
    {
        case Make_Compile:
            cmdStr = "compile";
            break;
        case Make_PreCompile:
            cmdStr = "preCompile";
            break;
        case Make_EXE:
            cmdStr = "exe";
            break;
        case Make_Clean:
            cmdStr = "clean";
            break;
    }
    if(pid == 0)
    {
        chdir(workDir.c_str());
        execl(makeDir.c_str(), "make", "-f", makefileDir.c_str(), 
                cmdStr.c_str(), fileName.c_str(), compiler.c_str(), extendName.c_str(), NULL);
        exit(1);
    }
    int status;
    waitpid(pid, &status, 0);
    if(WIFEXITED(status) && WEXITSTATUS(status)==0)
        return true;
    return false;
}
bool filterString(string filePath, string filter)
{
    ifstream fin;
    fin.open(filePath.c_str(), ios::in);
    string line;
    while(getline(fin, line))
    {
        int pos = line.find("//");
        if(pos != string::npos)
            line = line.substr(0, pos);        
        if(line.find(filter) !=  string::npos)
            return false;
    }
    return true;
    
}
bool preCompile()
{
    bool ret = true;
    string filePath = workDir + "/" + fileName.substr(9)+".cpp";
    for(auto filter : filterVec)
    {
        ret = filterString(filePath, filter);
        if(!ret)
            break;
    }
    ret = ret && make(Make_PreCompile);
    return ret;
}
bool compile()
{
    bool ret = make(Make_Compile);
    return ret;
}
bool execute()
{
    bool ret = make(Make_EXE);
    return ret;
}
bool clean()
{
    bool ret = make(Make_Clean);
    return ret;
}
bool runExe()
{
    pid_t pid = fork();
    if(pid == -1 )
        return false;
    if(pid == 0)
    {
        chdir(workDir.c_str());
        string exeFile = fileName.substr(9)+".exe";
        string path = "./" + exeFile;
        string inFile = fileName.substr(9)+".in";
        string outFile = fileName.substr(9)+".out1";
        freopen(inFile.c_str(), "r", stdin);
        freopen(outFile.c_str(), "w", stdout);
        execl(path.c_str(), exeFile.c_str(), NULL);
        printf("failed %s\n",exeFile.c_str());
        exit(1);
    }
    sleep(1);
    int status;
    if(waitpid(pid, &status, WNOHANG) == 0)
    {
        kill(pid, SIGKILL);
        return false;
    }
    if(WIFEXITED(status) && WEXITSTATUS(status)==0)
        return true;
    return false;
}
bool compare()
{
    string outPath = workDir + "/" + fileName.substr(9)+".out";
    string out1Path = workDir + "/" + fileName.substr(9)+".out1";
    ifstream fin, fin1;
    fin.open(outPath, ios::in);
    fin1.open(out1Path, ios::in);
    string str, str1;
    while(!fin.eof()||!fin1.eof())
    {
        fin>>str;
        fin1>>str1;
        if(str != str1)
            return false;
    }
    if(fin.eof() && fin1.eof())
        return true;
    return false;
}
void init_with_arg(int argc, char** argv)
{
}
int main(int argc, char** argv) {
    bool ret = preCompile();
    ret = ret && compile();
    ret = ret && execute();
    ret = ret && runExe();
    ret = ret && compare();
    printf("%d\n", ret);
    return 0;
}

