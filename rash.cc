#include "rash.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void ErrorCheck(bool condition, char *message) {
	if(condition)
		perror(message);
}
void ErrorCheckExit(bool condition, char *message) {
	if(condition) {
		perror(message);
		//exit(1);
	}
}



string getPwd() {
	char temp[4096];
	ErrorCheckExit(getcwd(temp, 4096) == NULL, "Couldn't get current directory");
	return string(temp);
}

Rash::Rash(){
	pwd = getPwd();
	uname = string(getenv("USER"));
	prompt = "[" + uname + "@ " + pwd + "]$ ";
	PATH = getenv("PATH");
	ErrorCheckExit(PATH.size() == 0, "Couldn't get PATH");
	//cout << "\033[2J\033[1;1H";
}
Rash::~Rash(){}


string Rash::promptAndWait() {
		string input;
		cout << "\33[2K\r";
		cout << prompt;
		cin >> input;
		return input;
}