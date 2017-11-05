#include "operator.h"

using namespace std;

Op::Op(){}
Op::~Op(){}
string Op::execute(){return "";}


PipeOp::PipeOp() {}
string PipeOp::execute() {
	string leftout = lhs->execute();
	return leftout;
}


CommandOp::CommandOp(vector<string> &input){
	this->input = input;
}

string CommandOp::execute() {
	if(input[0].size() == 0 || checkBuiltins())
		return "";
	
	string executable = findBin(input[0]);
	if(executable == "")
		return "rash: Command not found: " + input[0] + "\n";

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

bool CommandOp::checkBuiltins() {
	if(input[0] == "cd"){
		if(input.size() == 1)
			input.push_back("/home/" + string(getpwuid(getuid())->pw_name));
		if(chdir(input[1].c_str()) != -1)
			setenv(strdup("PWD"), input[1].c_str(), 1);
		else
			cout << "rash: No such file or directory: " + input[1] + "\n";
		return true;
	}
	if(input[0] == "exit")
		exit(1);

	return false;
}
