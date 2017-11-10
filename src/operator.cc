#include "operator.h"

using namespace std;

Op::Op(){}
Op::~Op(){}
string Op::execute() {
	return "";
}

NullOp::NullOp(){
	rhs = NULL;
	lhs = NULL;
}
NullOp::~NullOp(){}
string NullOp::execute() {
	return "";
}

OutputRedirOp::OutputRedirOp() {}
OutputRedirOp::~OutputRedirOp() {}
string OutputRedirOp::execute() {
	//fork
	//if the kid
		//fd = open target file
		//dup2(fd, 0)
	return "";
}

InputRedirOp::InputRedirOp() {}
InputRedirOp::~InputRedirOp() {}
string InputRedirOp::execute() {
	return "";
}

PipeOp::PipeOp() {}
string PipeOp::execute() {
}


CommandOp::CommandOp(vector<string> &input){
	rhs = new NullOp();
	lhs = new NullOp();
	this->input = input;
}

//info on piping to a buffer is from:
//https://stackoverflow.com/questions/7292642/grabbing-output-from-exec
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
		close(pipefds[0]);
		
		dup2(pipefds[1], STDOUT_FILENO);
		execv(executable.c_str(), args);
	} else {
		close(pipefds[1]);

		read(pipefds[0], (void *)buffer, 4096);
		waitpid(pid, NULL, WNOHANG);

		close(pipefds[0]);
		
		for(uint i = 0; i < sizeof(args)/sizeof(args[0]); i++) {
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