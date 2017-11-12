#include "rash.h"

using namespace std;

Rash::Rash(){
	pathdirs = splitStr(getEnv("PATH"), ':');
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
		input = splitStr(promptForInput(), ' ');
		interpret(input);
	}
}

void Rash::interpret(vector<string> &input) {
	input = expand(input);
	if(input.size() == 0){
		return;
	}
	if(input[0] == ""){
		perror("glob");
		return;
	}
	Op *root = parse(input);
	int rootfds[2] = {-1, -1};
	int rootpid = root->execute(rootfds);

	while(wait(NULL) > 0)
		continue;
	delete root;
}

//Pipes take precedence
//redirects are evaluated from left to right; must be added to tree from right to left
Op *Rash::parse(vector<string> &input) {
	//get last postition of an operator, prioritising pipes before redirects
	int nextOpPos = getLastPositionOf(input, "|");
	if (nextOpPos == -1) {
		nextOpPos = getLastPositionOf(input, "<>");
		if (nextOpPos == -1){
			return new CommandOp(input);
		}
	}

	//set that operator as root
	Op *nextOp;
	if (input[nextOpPos] == "|") {
		nextOp = new PipeOp();
	} else if (input[nextOpPos] == ">") {
		nextOp = new OutputRedirOp();
	} else if (input[nextOpPos] == "<") {
		nextOp = new InputRedirOp();
	}
	vector<string>::iterator start;
	vector<string>::iterator end;

	start = input.begin() + nextOpPos + 1;
	end = input.begin() + input.size();
	vector<string> newrhs(start, end);

	start = input.begin();
	end = input.begin() + nextOpPos;
	vector<string> newlhs(start, end);

	nextOp->rhs = parse(newrhs);
	nextOp->lhs = parse(newlhs);

	return nextOp;
}

//Clearline from:
//https://stackoverflow.com/questions/1508490/erase-the-current-printed-console-line
string Rash::promptForInput() {
	char *input = strdup("");
	string pwd = getPwd();
	if (pwd.substr(0, 6 + uname.size()) == "/home/" + uname)
		pwd.replace(0, 6 + uname.size(), "~");
	
	string prompt = "[" + uname + "@ " + pwd + "]$ ";
	char *output = readline(("\33[2K\r" + prompt).c_str());
	add_history(output);
	return string(output);
}

vector<string> Rash::expand(vector<string> &input) {
	int vecSize = input.size();
	for(int i = 0; i < vecSize; i++){
		if (input[i][0] == '~')
			input[i].replace(0, 1, "/home/" + uname);
		else if (input[i][0] == '$')
			input[i] = getEnv(input[i].substr(1, input[i].size() - 1));

		if(input[i].find_first_of("*") == string::npos)
			continue;

		glob_t result;
		if(glob(input[i].c_str(), GLOB_NOCHECK, NULL, &result) == 0){
			input[i] = "";
			for(uint j = 0; j < result.gl_pathc; j++)
				input.insert(input.begin() + i + 1, string(result.gl_pathv[j]));
			input.erase(input.begin() + i);
		} else {
			input[0] = "";
			return input;
		}
	}
	return input;
}