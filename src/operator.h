#ifndef OP_H
#define OP_H
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


using namespace std;

class Op{
public:
	Op();
	~Op();

	virtual string execute() {return "";}

	Op *lhs;
	Op *rhs;
};

class CommandOp: public Op{
public:
	CommandOp(vector<string> &input, vector<string> &pathdirs);
	~CommandOp();

	virtual string execute();

	string executable;
	vector<string> input;
};

class PipeOp: public Op{
public:
	PipeOp();
	~PipeOp();

	virtual string execute() {return "";}
};
#endif
