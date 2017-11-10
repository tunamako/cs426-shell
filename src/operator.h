#ifndef OP_H
#define OP_H
#include "helpers.h"

#include <sys/wait.h>
#include <pwd.h>

class Op {
public:
	Op();
	~Op();
	virtual std::string execute();

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
	virtual std::string execute();
	virtual std::string type() {
		return "NullOp";
	}
};

class OutputRedirOp: public Op {
public:
	OutputRedirOp();
	~OutputRedirOp();
	virtual std::string execute();
	virtual std::string type() {
		return "OutputRedirOp";
	}
};

class InputRedirOp: public Op {
public:
	InputRedirOp();
	~InputRedirOp();
	virtual std::string execute();
	virtual std::string type() {
		return "InputRedirOp";
	}
};

class PipeOp: public Op {
public:
	PipeOp();
	virtual std::string execute();
	virtual std::string type() {
		return "PipeOp";
	}
};

class CommandOp: public Op {
public:
	CommandOp(std::vector<std::string> &input);
	virtual std::string execute();
	bool checkBuiltins();

	std::vector<std::string> input;
	virtual std::string type() {
		return "CommandOp";
	}
};

#endif
