#ifndef RASH_H
#define RASH_H
#include <string>
#include <vector>

using namespace std;
class Rash{
public:
	Rash();
	~Rash();
	
	void run();
	void execute(string cmd);
	string promptForInput();

	vector<string> commands;
	vector<string> operators;
	vector<string> history;
	vector<string> queue;
	vector<string> pathdirs;

	string pwd;
	string uname;
	string prompt;

};

#endif