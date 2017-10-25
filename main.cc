#include "rash.h"

int main() {
	Rash *shell = new Rash();
	while(true)
		shell->promptAndWait();
}