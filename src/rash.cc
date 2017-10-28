#include "rash.h"

void ErrorCheckExit(bool condition, char *message) {
	if(condition) {
		perror(message);
		exit(1);
	}
}
vector<string> splitStr(char *aString, char *delims) {
	vector<string> ret;
	char *temp = strtok(aString, delims);
	while(temp != NULL) {
		ret.push_back(string(temp));
		temp = strtok(NULL, delims);
	}
	return ret;
}
string getPwd() {
	char temp[4096];
	ErrorCheckExit(getcwd(temp, 4096) == NULL, strdup("Couldn't get current directory"));
	return string(temp);
}



//Clearscreen codes from 
//https://stackoverflow.com/questions/4062045/clearing-terminal-in-linux-with-c-code
Rash::Rash(){
	pwd = getPwd();
	uname = string(getenv("USER"));
	prompt = "[" + uname + "@ " + pwd + "]$ ";
	pathdirs = splitStr(strdup(getenv("PATH")), strdup(":"));
	ErrorCheckExit(pathdirs.size() == 0, strdup("Couldn't get PATH"));
	//cout << "\033[2J\033[1;1H";
}
Rash::~Rash(){}

void Rash::run(){
	vector<string> input;
	while(true) {
		input = splitStr(promptForInput(), strdup(" "));
		cout << interpret(input);
	}
}

string Rash::interpret(vector<string> &input) {
	Op *root = new CommandOp(input, pathdirs);
	string output = root->execute();
	delete root;
	return output;
}

char *Rash::promptForInput() {
		char *input = strdup("");
		while(strlen(input) == 0) {
			cout << "\33[2K\r";
			cout << prompt;
			cin.getline(input, 256);
		}
		return input;
}