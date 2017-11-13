#include "helpers.h"
#include <sstream>

using namespace std;

void ErrorCheckExit(bool condition, string message) {
	if(condition) {
		perror(message.c_str());
		exit(1);
	}
}
void ErrorCheck(bool condition, string message) {
	if(condition)
		perror(message.c_str());
}

//Some info on stringstreams from
//https://stackoverflow.com/questions/11719538/how-to-use-stringstream-to-separate-comma-separated-strings
vector<string> splitStr(string aString, char delimiter) {
	vector<string> ret;
	stringstream ss(aString);
	string temp;
	while (getline(ss, temp, delimiter))
		ret.push_back(temp);
	return ret;
}

string getEnv(string varname) {
	extern char **environ;
	int i = 0;
	char *next = environ[i];

	while(next){
		if(string(next).substr(0,varname.size()) == varname) 
			return string(next).substr(varname.size() + 1, strlen(next));
		next = environ[i++];
	}
	return "";
}

string getPwd() {
	char temp[4096];
	ErrorCheckExit(getcwd(temp, 4096) == NULL, "getcwd");
	return string(temp);
}

char **convertVector(vector<string> &aVector) {
	char **ret = new char*[aVector.size()];

	for(uint i = 0; i < aVector.size(); i++) {
		int strsize = aVector[i].size();
		char *temp = strdup(aVector[i].c_str());
		ret[i] = new char[strsize];
		strncpy(ret[i], temp, strsize);
		ret[i][strsize] = 0;
	}
	ret[aVector.size()] = NULL;

	return ret;
}

string findBin(string cmd) {
	struct stat buf;
	if(stat(cmd.c_str(), &buf) == 0)
		return cmd;

	string executable;
	vector<string> pathdirs = splitStr(getEnv("PATH"),':');
	for(auto dir : pathdirs) {
		executable = dir + "/" + cmd;
		if(stat(executable.c_str(), &buf) == 0)
			return executable;
	}
	return "";
}

//Getting the index of the iterator on return is from:
//https://stackoverflow.com/questions/24997910/get-index-in-vector-from-reverse-iterator
int getLastPositionOf(vector<string> &input, string delims) {
	vector<string>::reverse_iterator iter = input.rbegin();

	while(iter != input.rend()) {
		for(auto op : delims) {
			string temp(1, op);
			if(*iter == temp)
				return distance(input.begin(), iter.base()) - 1;
		}
		*iter++;
	}
	return -1;
}