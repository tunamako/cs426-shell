#include "rash.h"

void ErrorCheckExit(bool condition, char *message) {
	if(condition) {
		perror(message);
		delete message;
		exit(1);
	}
	delete message;
}
vector<string> splitStr(string aString, char *delims) {
	vector<string> ret;
	char *temp = strtok(strdup(aString.c_str()), delims);
	while(temp != NULL) {
		ret.push_back(string(temp));
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
	ErrorCheckExit(true, strdup(("Couldnt find " + varname + " variable").c_str()));
}
string getPwd() {
	char temp[4096];
	ErrorCheckExit(getcwd(temp, 4096) == NULL, strdup("Couldn't get current directory"));
	return string(temp);
}



Rash::Rash(){
	pathdirs = splitStr(getEnv("PATH"), strdup(":"));
	ErrorCheckExit(pathdirs.size() == 0, strdup("Couldn't get PATH"));

	uname = getpwuid(getuid())->pw_name;
	pwd = "/home/" + uname;
	chdir(pwd.c_str());
}
Rash::~Rash(){}

void Rash::run(){
	vector<string> input;
	while(true) {
		input = splitStr(promptForInput(), strdup(" "));
		input = expand(input);
		cout << interpret(input);
	}
}

vector<string> Rash::expand(vector<string> &input) {
	for(uint i = 0; i < input.size(); i++){
		if (input[i][0] == '~')
			input[i].replace(0, 1, "/home/" + uname);
		else if (input[i][0] == '$'){
			input[i] = getEnv(input[i].substr(1, input[i].size() - 1));
		}
	}
	return input;
}


string Rash::changedir(string dir) {
	struct stat buf;
	if(dir.size() == 0)
		pwd = "/home/" + uname;
	else if(stat(dir.c_str(), &buf) == 0)
		pwd = (dir[0] == '/')
			? dir
			: pwd + "/" + dir;
	else
		return "cd: no such file or directory: " + dir + "\n";
	
	chdir(pwd.c_str());
	return "";
}

string Rash::interpret(vector<string> &input) {
	//Check for builtin functions
	if(input[0] == "cd") 
		return (input.size() == 1)
			? changedir("")
			: changedir(input[1]);
	if(input[0] == "exit")
		exit(1);
	//Use exec
	Op *root = new CommandOp(input, pathdirs);
	string output = "";
	output = root->execute();
	delete root;
	return output;
}

string Rash::promptForInput() {
	char *input = strdup("");
	while(strlen(input) == 0) {
		cout << "\33[2K\r";
		if(pwd == "/home/" + uname) 
			cout << "[" + uname + "@ ~]$ ";
		else
			cout << "[" + uname + "@ " + splitStr(pwd, strdup("/")).back() + "]$ ";
		cin.getline(input, 256);
	}
	return string(input);
}