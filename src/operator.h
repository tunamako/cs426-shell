#ifndef OP_H
#define OP_H
#include "helpers.h"

#include <sys/wait.h>
#include <pwd.h>

class Op{
public:
	Op();
	~Op();
	virtual std::string execute();

	Op *lhs;
	Op *rhs;
};

class OutputRedirOp: public Op{
public:
	OutputRedirOp();
	~OutputRedirOp();
};
class InputRedirOp: public Op{
public:
	InputRedirOp();
	~InputRedirOp();
};

class PipeOp: public Op{
public:
	PipeOp();
	virtual std::string execute();
};

class CommandOp: public Op{
public:
	CommandOp(std::vector<std::string> &input);
	virtual std::string execute();
	bool checkBuiltins();

	std::vector<std::string> input;
};

#endif
