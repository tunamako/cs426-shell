#include "rash.h"

using namespace std;

Rash::Rash(){
	pathdirs = splitStr(getEnv("PATH"), strdup(":"));
	ErrorCheckExit(pathdirs.size() == 0, "Couldn't get PATH");

	uname = getpwuid(getuid())->pw_name;
	const char* home = ("/home/" + uname).c_str();
	chdir(home);
	setenv(strdup("PWD"), home, 1);
}
Rash::~Rash(){}

void Rash::run(){
	vector<string> input;
	while(true) {
		input = splitStr(promptForInput(), strdup(" "));
		cout << interpret(input);
	}
}

//This should be recursive
string Rash::interpret(vector<string> &input) {
	input = expand(input);
	if(input.size() == 0)
		return "";
	if(input[0] == "")
		perror("glob");

	//Use exec
	Op *root = parse(input);
	string output = root->execute();
	delete root;
	return output;
}

Op *Rash::parse(vector<string> &input) {
	return new CommandOp(input);
}

string Rash::promptForInput() {
	char *input = strdup("");
	string pwd = getPwd();
	if (pwd.substr(0, 6 + uname.size()) == "/home/" + uname)
		pwd.replace(0, 6 + uname.size(), "~");
	
	string prompt = "[" + uname + "@ " + pwd + "]$ ";
	cout << "\33[2K\r" + prompt;

	cin.getline(input, 256);
	return string(input);
}

string Rash::globString(string input){
	glob_t result;
	if(glob(input.c_str(), GLOB_NOCHECK, NULL, &result) == 0){
		input = "";

		for(uint j = 0; j < result.gl_pathc; j++) {
			if (j == result.gl_pathc - 1)
				input += string(result.gl_pathv[j]);
			else
				input += string(result.gl_pathv[j]) + " ";
		}
	} else {
		return "";
	}
	return input;
}

vector<string> Rash::expand(vector<string> &input) {
	int vecSize = input.size();
	for(uint i = 0; i < vecSize; i++){
		if (input[i][0] == '~')
			input[i].replace(0, 1, "/home/" + uname);
		else if (input[i][0] == '$')
			input[i] = getEnv(input[i].substr(1, input[i].size() - 1));

		if(input[i].find_first_of("*") == string::npos)
			continue;
		input[i] = globString(input[i]);

		if(input[i] != ""){
			for(auto v : splitStr(input[i], " ")) 
				input.insert(input.begin() + i + 1, v);
			input.erase(input.begin() + i);
		} else {
			input[0] = "";
			return input;
		}
	}
	return input;
}
