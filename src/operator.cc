#include "rash.h"
#include "operator.h"
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <vector>

char **convertVector(vector<string> &aVector) {
	char **ret = new char*[aVector.size()];

	for(uint i = 0; i < aVector.size(); i++) {
		char *temp = strdup(aVector[i].c_str());
		ret[i] = new char[aVector[i].size()];
		strncpy(ret[i], temp, aVector[i].size());
	}
	ret[aVector.size()] = NULL;
	
	return ret;
}
string findBin(string cmd, vector<string> &pathdirs) {
	struct stat buf;
	string executable;

	if(stat(cmd.c_str(), &buf) == 0){
		return cmd;
	}

	for(auto dir : pathdirs) {
		executable = dir + "/" + cmd;
		if(stat(executable.c_str(), &buf) == 0){
			return executable;
		}
	}
	cout << "rash: Command not found: " + cmd << endl;
	return "";
}

Op::Op(){}
Op::~Op(){}

PipeOp::PipeOp(){}
PipeOp::~PipeOp(){}


CommandOp::CommandOp(vector<string> &input, vector<string> &pathdirs){
	this->executable = findBin(input[0], pathdirs);
	this->input = input;
}
CommandOp::~CommandOp(){}

string CommandOp::execute() {
	if(!executable.size()){
		return "";
	}
	int pipefds[2];
	char buffer[4096];
	pipe(pipefds);
	memset(buffer, 0, 4096);

	int pid = fork();	
	if(pid == 0) {
		dup2(pipefds[1], STDOUT_FILENO);
		close(pipefds[0]);

		execv(executable.c_str(), convertVector(input));

	} else {
		close(pipefds[1]);
		read(pipefds[0], (void *)buffer, 4096);
		wait(NULL);
		return string(strdup(buffer));
	}
}