#include "rash.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

string getPwd() {
	char temp[256];
	getcwd(temp, 256);
	return string(temp);
}

void ErrorCheckExit(bool condition, char *message) {
	if(condition) {
		perror(message);
		exit(1);
	}
}

Rash::Rash(){
	this->pwd = getPwd();
	this->PATH = getenv("PATH");
	ErrorCheckExit(!this->PATH.size(), "PATH variable couldn't be found");

	//cout << "\033[2J\033[1;1H";
}
Rash::~Rash(){}


string Rash::promptAndWait() {
		string input;
		cout << "\33[2K\r";
		cout << "[testing@ " + pwd + "]$ ";
		cin >> input;
		return input;
}