#ifndef OP_H
#define OP_H
#include "helpers.h"

#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>

class Op {
public:
	Op();
	~Op();
	virtual int execute(int inoutfds[2]);
	virtual std::string type() {
		return "Op";
	}
	virtual std::vector<std::string> getInput() { return (std::vector<std::string>)NULL; }
	Op *lhs;
	Op *rhs;
};

class NullOp: public Op {
public:
	NullOp();
	~NullOp();
	virtual int execute(int inoutfds[2]);
	virtual std::string type() {
		return "NullOp";
	}
};

class OutputRedirOp: public Op {
public:
	OutputRedirOp();
	~OutputRedirOp();
	virtual int execute(int inoutfds[2]);
	virtual std::string type() {
		return "OutputRedirOp";
	}
};

class InputRedirOp: public Op {
public:
	InputRedirOp();
	~InputRedirOp();
	virtual int execute(int inoutfds[2]);
	virtual std::string type() {
		return "InputRedirOp";
	}
};

class PipeOp: public Op {
public:
	PipeOp();
	virtual int execute(int inoutfds[2]);
	virtual std::string type() {
		return "PipeOp";
	}
};

class CommandOp: public Op {
public:
	CommandOp(std::vector<std::string> &input);
	virtual int execute(int inoutfds[2]);
	bool checkBuiltins();
	virtual std::string type() {
		return "CommandOp";
	}
	virtual std::vector<std::string> getInput() { 
		return this->input; 
	}

	std::vector<std::string> input;
};

#endif
