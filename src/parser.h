#ifndef PARSE_H
#define PARSE_H
#include "rash.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <vector>


using namespace std;
class Parser{
public:
	Parser(vector<string> pathdirs);
	~Parser();

	string interpret(vector<string> input);
	string findBin(string cmd);

	vector<string> pathdirs;
};



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
	CommandOp(vector<string> input, string executable);
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
