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
	char **convertVector(vector<string> &aVector);
	string findBin(string cmd, vector<string> &pathdirs);
	virtual string execute();

	Op *lhs;
	Op *rhs;
};

class PipeOp: public Op{
public:
	PipeOp();
	~PipeOp();
};

class CommandOp: public Op{
public:
	CommandOp(vector<string> &input, vector<string> &pathdirs);
	virtual string execute();

	string executable;
	vector<string> input;
};

#endif
