#include "gtest/gtest.h"
#include "rash.h"
#include <string.h>
#include <unistd.h> 
#include <stdio.h>
#include <string>

TEST(ShellTest, testCreation){
	Rash *shell = new Rash();

	EXPECT_EQ(shell->pwd, "/home/michael/Documents/cs426/shell");
	EXPECT_EQ(shell->uname, "michael");
	EXPECT_EQ(shell->pathdirs[0], "/usr/local/sbin");
	EXPECT_EQ(shell->pathdirs[2], "/usr/bin");

	Rash *anothershell = new Rash();
	EXPECT_EQ(anothershell->pwd, "/home/michael/Documents/cs426/shell");
	EXPECT_EQ(anothershell->uname, "michael");
	EXPECT_EQ(anothershell->pathdirs[0], "/usr/local/sbin");
	EXPECT_EQ(anothershell->pathdirs[2], "/usr/bin");
}

TEST(ShellTest, testBinSearch) {
	Rash *shell = new Rash();
	EXPECT_EQ(shell->findBin("/bin/ls"), "/bin/ls");
	EXPECT_EQ(shell->findBin("ls"), "/usr/bin/ls");
	EXPECT_EQ(shell->findBin("/bin/cat"), "/bin/cat");
	EXPECT_EQ(shell->findBin("cat"), "/usr/bin/cat");
}

TEST(ShellTest, testSingleExecution) {
	Rash *shell = new Rash();
	char *args[1];
	args[0] = NULL;
	EXPECT_EQ(shell->execute(strdup("ls"), args), "LICENSE\rmakefile\rparser.o\rrequirements\rmain\rparser.cc\rrash.cc\rtest_hw.cc\rmain.cc\rparser.h\rrash.h\rtest_hw.o\rmain.o\rparser.h.gch\rrash.o\rtestRunner");
}


GTEST_API_ int main(int argc, char *argv[]){
	printf("Running tests...\n\r");
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
