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
	string execute(string cmd, char *args[]);
	string findBin(string cmd);
	char *promptForInput();
	
	vector<string> history;
	vector<string> queue;
	vector<string> pathdirs;

	string pwd;
	string uname;
	string prompt;

};

#endif