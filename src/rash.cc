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
	pwd = (pwd == "/home/" + uname)? "~" : pwd;
	string prompt = "[" + uname + "@ " + pwd + "]$ ";

	cout << "\33[2K\r" + prompt;
	cin.getline(input, 256);
	return string(input);
}

vector<string> Rash::expand(vector<string> &input) {
	for(uint i = 0; i < input.size(); i++){
		if (input[i][0] == '~')
			input[i].replace(0, 1, "/home/" + uname);
		else if (input[i][0] == '$')
			input[i] = getEnv(input[i].substr(1, input[i].size() - 1));

		int ast = input[i].find_first_of("*");
		if(ast == string::npos)
			continue;

		string prefix = input[i].substr(0, ast - 1);
		glob_t result;
		if(glob(input[i].c_str(), GLOB_NOCHECK, NULL, &result) == 0){
			input[i] = "";

			for(uint i = 0; i < result.gl_pathc; i++) {
				input[i] += prefix + string(result.gl_pathv[i]) + " ";
				cout << input[i] << endl;
			}
		}
		else {
			input[0] == "";
		}
	}
	return input;
}
