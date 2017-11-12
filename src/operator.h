#ifndef OP_H
#define OP_H
#include "helpers.h"

#include <sys/wait.h>
#include <pwd.h>

class Op {
public:
	Op();
	~Op();
	virtual int execute(int inoutfds[2]);

	Op *lhs;
	Op *rhs;
	virtual std::string type() {
		return "Op";
	}
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

	std::vector<std::string> input;
	virtual std::string type() {
		return "CommandOp";
	}
};

#endif
