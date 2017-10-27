#include "rash.h"
#include <unistd.h> 
int main() {
	Rash *shell = new Rash();
	shell->run();
	//execl("/usr/bin/ls", "/");
}