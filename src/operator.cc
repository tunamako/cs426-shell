#include "operator.h"

using namespace std;

Op::Op(){}
Op::~Op(){}
int Op::execute(int infd, int outfd) {
}

NullOp::NullOp(){
	rhs = NULL;
	lhs = NULL;
}
int NullOp::execute(int infd, int outfd) {}

OutputRedirOp::OutputRedirOp() {}
int OutputRedirOp::execute(int infd, int outfd) {
	char * filename = strdup(rhs->getInput()[0].c_str());
	int outfile = open(filename, O_RDWR | O_TRUNC | O_CREAT, 00666);
	
	if(outfile < 0) {
		cerr << "rash: failed to open file: " + string(filename) << endl;
		return -1;
	}
	delete filename;

	lhs->execute(infd, outfile);
}

InputRedirOp::InputRedirOp() {}
int InputRedirOp::execute(int infd, int outfd) {
	char * filename = strdup(rhs->getInput()[0].c_str());
	int infile = open(filename, O_RDWR);
	
	if(infile < 0) {
		cerr << "rash: failed to open file: " + string(filename) << endl;
		return -1;
	}
	delete filename;

	lhs->execute(infile, outfd);
}

PipeOp::PipeOp() {}
int PipeOp::execute(int infd, int outfd) {
	int pipefds[2];
	ErrorCheck(pipe(pipefds) < 0, "pipe");

	lhs->execute(-1, pipefds[1]);
	rhs->execute(pipefds[0], outfd);
}


CommandOp::CommandOp(vector<string> &input){
	rhs = new NullOp();
	lhs = new NullOp();
	this->input = input;
}

int CommandOp::execute(int infd, int outfd) {
	if(input[0].size() == 0 || checkBuiltins())
		return -1;

	string executable = findBin(input[0]);
	if(executable == "") 
		cerr << "rash: Command not found: " + input[0] + "\n";

	char **args = convertVector(input);

	int pid = fork();
	if(pid == 0){
		if(infd != -1) {
			ErrorCheck(dup2(infd, STDIN_FILENO) < 0, "dup2");
			close(infd);
		}
		if(outfd != -1) {
			ErrorCheck(dup2(outfd, STDOUT_FILENO) < 0, "dup2");
			close(outfd);
		}
		execv(executable.c_str(), args);
	} else {
		close(infd);
		close(outfd);
		return pid;
	}
}

bool CommandOp::checkBuiltins() {
	if(input[0] == "cd"){
		if(input.size() == 1)
			input.push_back((getenv("HOME")));
		if(chdir(input[1].c_str()) != -1)
			setenv("PWD", input[1].c_str(), 1);
		else
			cout << "rash: No such file or directory: " + input[1] + "\n";
		return true;
	}
	if(input[0] == "exit")
		exit(1);

	return false;
}