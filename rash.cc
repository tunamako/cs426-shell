#include "rash.h"
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void ErrorCheck(bool condition, char*message) {
	if(condition)
		perror(message);
}
void ErrorCheckExit(bool condition, char *message) {
	if(condition) {
		perror(message);
		exit(1);
	}
}
vector<string> splitStr(char *aString, char *delims) {
	vector<string> ret;
	char *temp = strtok(aString, delims);

	while(temp != NULL) {
		ret.push_back(string(temp));
		temp = strtok(NULL, delims);
	}
	return ret;
}
string getPwd() {
	char temp[4096];
	ErrorCheckExit(getcwd(temp, 4096) == NULL, strdup("Couldn't get current directory"));
	return string(temp);
}



//Clearscreen codes from https://stackoverflow.com/questions/4062045/clearing-terminal-in-linux-with-c-code
Rash::Rash(){
	pwd = getPwd();
	uname = string(getenv("USER"));
	prompt = "[" + uname + "@ " + pwd + "]$ ";
	pathdirs = splitStr(getenv("PATH"), strdup(":"));
	ErrorCheckExit(pathdirs.size() == 0, strdup("Couldn't get PATH"));
	//cout << "\033[2J\033[1;1H";
}
Rash::~Rash(){}

void Rash::run(){
	vector<string> input;
	string cmd;
	while(true) {
		input = splitStr(promptForInput(), strdup(" "));
		char *args[input.size()];
		cmd = input[0];

		for(uint i = 1; i < input.size(); i++) {
			args[i] = strdup(input[i].c_str());
		}
		args[input.size()] = NULL;
		execute(cmd, args);
	}
}

void Rash::execute(string cmd, char *args[]) {
	struct stat buf;
	string executable = "";

	for(auto dir : pathdirs) {
		executable = dir + "/" + cmd;
		if(stat(executable.c_str(), &buf) == 0) {
			int pid = fork();
			if(pid == 0) {
				execv(executable.c_str(), args);
				//TODO: pipe output here to be controlled for more generality
			}
			wait(NULL);
			
			return;
		}
	}
	cout << "rash: Command not found" << endl;
}

char *Rash::promptForInput() {
		char *input = strdup("");
		while(strlen(input) == 0) {
			cout << "\33[2K\r";
			cout << prompt;
			cin.getline(input, 256);
		}
		return input;
}

