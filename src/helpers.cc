#include "helpers.h"

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

vector<string> splitStr(string aString, char *delims) {
	vector<string> ret;
	char *temp = strtok(strdup(aString.c_str()), delims);
	while(temp != NULL) {
		ret.push_back(string(strdup(temp)));
		temp = strtok(NULL, delims);
	}
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
	ErrorCheckExit(getcwd(temp, 4096) == NULL, "Couldn't get current directory");
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
	vector<string> pathdirs = splitStr(getEnv("PATH"),":");
	for(auto dir : pathdirs) {
		executable = dir + "/" + cmd;
		if(stat(executable.c_str(), &buf) == 0)
			return executable;
	}
	return "";
}