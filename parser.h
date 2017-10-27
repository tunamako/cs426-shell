#ifndef PARSE_H
#define PARSE_H

using namespace std;
class Parser{
public:
	Parser();
	~Parser();
};

#endif


class Op{
public:
	Op();
	~Op();
	Op *lhs;
	Op *rhs;
};

class CommandOp: public Op{
public:
	CommandOp();
	~CommandOp();
};

class PipeOp: public Op{
public:
	PipeOp();
	~PipeOp();
};
