#ifndef RASH_H
#define RASH_H
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
class Rash{
public:
	Rash();
	~Rash();
	
	void run();
	char *promptForInput();
	string interpret(vector<string> &input);
	
	vector<string> history;
	vector<string> queue;
	vector<string> pathdirs;
	string pwd;
	string uname;
	string prompt;

};

#endif