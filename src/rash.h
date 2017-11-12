#ifndef RASH_H
#define RASH_H
#include "rash.h"
#include "operator.h"
#include "helpers.h"

#include <glob.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <algorithm>
class Rash{
public:
	Rash();
	~Rash();
	
	void run();
	std::string promptForInput();
	void interpret(std::vector<std::string> &input);
	std::vector<std::string> expand(std::vector<std::string> &input);
	Op *parse(std::vector<std::string> &input);
	
	std::vector<std::string> history;
	std::vector<std::string> queue;
	std::vector<std::string> pathdirs;
	std::string uname;

};

#endif