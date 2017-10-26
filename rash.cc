#include "rash.h"
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void ErrorCheck(bool condition, string message) {
	if(condition)
		perror(message.c_str());
}
void ErrorCheckExit(bool condition, string message) {
	if(condition) {
		perror(message.c_str());
		exit(1);
	}
}

string getPwd() {
	char temp[4096];
	ErrorCheckExit(getcwd(temp, 4096) == NULL, "Couldn't get current directory");
	return string(temp);
}
vector<string> getPATH() {
	vector<string> pathdirs;
	char *PATH = getenv("PATH");
	char *temp = strtok(PATH, ":");

	while(temp) {
		pathdirs.push_back(string(temp));
		temp = strtok(NULL, ":");
	}
	ErrorCheckExit(pathdirs.size() == 0, "Couldn't get PATH");
	return pathdirs;
}


//Clearscreen codes from https://stackoverflow.com/questions/4062045/clearing-terminal-in-linux-with-c-code
Rash::Rash(){
	pwd = getPwd();
	uname = string(getenv("USER"));
	prompt = "[" + uname + "@ " + pwd + "]$ ";
	pathdirs = getPATH();
	//cout << "\033[2J\033[1;1H";
}
Rash::~Rash(){}

void Rash::run(){
	string input;
	while(true) {
		execute(promptForInput());
	}
}

void Rash::execute(string cmd) {
	struct stat buf;
	string executable = "";

	for(auto dir : pathdirs) {
		executable = dir + "/" + cmd;
		if(stat(executable.c_str(), &buf) == 0) {
			int pid = fork();
			if(pid == 0) {
				execl(executable.c_str(), cmd.c_str(), pwd.c_str());
				//TODO: pipe output here to be controlled for more generality
			}
			wait(NULL);
			return;
		}
	}
	ErrorCheckExit(true, cmd + ": Command not found");
}

string Rash::promptForInput() {
		string input;
		cout << "\33[2K\r";
		cout << prompt;
		cin >> input;
		return input;
}

