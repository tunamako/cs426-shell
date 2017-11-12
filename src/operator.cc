#include "operator.h"

using namespace std;

//Have all executes return file descriptors for their output!

Op::Op(){}
Op::~Op(){}
int Op::execute(int inoutfds[2]) {
}

NullOp::NullOp(){
	rhs = NULL;
	lhs = NULL;
}
NullOp::~NullOp(){}
int NullOp::execute(int inoutfds[2]) {
}

OutputRedirOp::OutputRedirOp() {}
OutputRedirOp::~OutputRedirOp() {}
int OutputRedirOp::execute(int inoutfds[2]) {
	char * filename = strdup(rhs->getInput()[0].c_str());
	int outfile = open(filename, O_RDWR | O_TRUNC | O_CREAT, 00666);
	
	if(outfile < 0) {
		cerr << "rash: failed to open file: " + string(filename) << endl;
		return -1;
	}
	delete filename;

	int lhsfds[2] = {inoutfds[0], outfile};

	lhs->execute(lhsfds);
}

InputRedirOp::InputRedirOp() {}
InputRedirOp::~InputRedirOp() {}
int InputRedirOp::execute(int inoutfds[2]) {
}

PipeOp::PipeOp() {}

//inoutfds specify the optional file descriptors that this particular pipe
//should input from and output to. (input is ignored as pipes always take input from the lhs)
//pipefds serves as the "bridge" between the lhs and rhs of the pipe operator
//lhsfds are like inoutfds, but specifying the lhs's input and output fds
int PipeOp::execute(int inoutfds[2]) {
	int pipefds[2];
	pipe(pipefds);

	int lhsfds[2] = {-1, pipefds[1]};
	int rhsfds[2] = {pipefds[0], inoutfds[1]};

	lhs->execute(lhsfds);
	rhs->execute(rhsfds);
}


CommandOp::CommandOp(vector<string> &input){
	rhs = new NullOp();
	lhs = new NullOp();
	this->input = input;
}

//info on piping to a buffer is from:
//https://stackoverflow.com/questions/7292642/grabbing-output-from-exec
int CommandOp::execute(int inoutfds[2]) {
	if(input[0].size() == 0 || checkBuiltins())
		return -1;

	string executable = findBin(input[0]);
	if(executable == "") 
		cerr << "rash: Command not found: " + input[0] + "\n";

	char **args = convertVector(input);

	int pid = fork();
	if(pid == 0){
		if(inoutfds[0] != -1) {
			dup2(inoutfds[0], STDIN_FILENO);
			close(inoutfds[0]);
		}
		if(inoutfds[1] != -1) {
			dup2(inoutfds[1], STDOUT_FILENO);
			close(inoutfds[1]);
		}
		execv(executable.c_str(), args);
	} else {
		if(inoutfds[0] != -1) {
			close(inoutfds[0]);
		}
		if(inoutfds[1] != -1) {
			close(inoutfds[1]);
		}
		return pid;
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