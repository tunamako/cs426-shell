#ifndef RASH_H
#define RASH_H
#include <string>
#include <vector>

using namespace std;
class Rash{
public:
	Rash();
	~Rash();
	
	string promptAndWait();

	vector<string> commands;
	vector<string> operators;
	vector<string> history;
	vector<string> queue;

	string pwd;
	string uname;
	string prompt;
	string PATH;

};

#endif