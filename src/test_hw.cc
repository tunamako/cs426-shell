#include "gtest/gtest.h"
#include "rash.h"
#include "operator.h"


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
	Op *oper = new Op();

	vector<string> pathdirs = shell->pathdirs;

	EXPECT_EQ(oper->findBin("/bin/ls", pathdirs), "/bin/ls");
	EXPECT_EQ(oper->findBin("ls", pathdirs), "/usr/bin/ls");
	EXPECT_EQ(oper->findBin("/bin/cat", pathdirs), "/bin/cat");
	EXPECT_EQ(oper->findBin("cat", pathdirs), "/usr/bin/cat");
}

TEST(ShellTest, testVecConversion) {
	Op *oper = new Op();

	vector<string> test = {"aaaa", "b", "c", "d"};
	char **result = oper->convertVector(test);

	EXPECT_EQ(string(result[0]), test[0]);
	EXPECT_EQ(string(result[3]), test[3]);
	EXPECT_EQ(strlen(result[0]), 4);
}



/*
TEST(ShellTest, testSingleExecution) {
	Rash *shell = new Rash();
	char *args[1];
	args[0] = NULL;
	EXPECT_EQ(shell->execute(strdup("ls"), args), "LICENSE\rmakefile\rparser.o\rrequirements\rmain\rparser.cc\rrash.cc\rtest_hw.cc\rmain.cc\rparser.h\rrash.h\rtest_hw.o\rmain.o\rparser.h.gch\rrash.o\rtestRunner");
}
*/

GTEST_API_ int main(int argc, char *argv[]){
	printf("Running tests...\n\r");
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
