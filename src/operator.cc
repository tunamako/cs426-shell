#include "operator.h"


char **Op::convertVector(vector<string> &aVector) {
	char **ret = new char*[aVector.size()];

	for(uint i = 0; i < aVector.size(); i++) {
		char *temp = strdup(aVector[i].c_str());
		ret[i] = new char[strlen(temp)];
		strncpy(ret[i], temp, strlen(temp));
	}
	ret[aVector.size()] = NULL;

	return ret;
}
string Op::findBin(string cmd, vector<string> &pathdirs) {
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
string Op::execute(){return "";}


PipeOp::PipeOp() {}
string PipeOp::execute() {
	string leftout = lhs->execute();
	return leftout;
}


CommandOp::CommandOp(vector<string> &input, vector<string> &pathdirs){
	this->input = input;
	this->pathdirs = pathdirs;
}

string CommandOp::execute() {
	if(input[0].size() == 0){
		return "";
	}
	string executable = findBin(input[0], pathdirs);

	int pipefds[2];
	char buffer[4096];
	char **args = convertVector(input);

	pipe(pipefds);
	memset(buffer, 0, 4096);

	int pid = fork();	
	if(pid == 0) {
		dup2(pipefds[1], STDOUT_FILENO);
		close(pipefds[0]);
		execv(executable.c_str(), args);
	} else {
		close(pipefds[1]);
		read(pipefds[0], (void *)buffer, 4096);
		wait(NULL);

		for(uint i = 0; i < input.size(); i++) {
			delete[] args[i];
		}
		delete[] args;
		return string(strdup(buffer));
	}
}