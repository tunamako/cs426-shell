#include "gtest/gtest.h"
#include "rash.h"
#include "operator.h"
#include "helpers.h"

using namespace std;
TEST(ShellTest, testCreation){
	Rash *shell = new Rash();

	EXPECT_EQ(shell->uname, "michael");
	EXPECT_EQ(shell->pathdirs[0], "/usr/local/sbin");
	EXPECT_EQ(shell->pathdirs[2], "/usr/bin");

	Rash *anothershell = new Rash();
	EXPECT_EQ(anothershell->uname, "michael");
}
TEST(ShellTest, testBinSearch) {
	EXPECT_EQ(findBin("/bin/ls"), "/bin/ls");
	EXPECT_EQ(findBin("ls"), "/usr/bin/ls");
	EXPECT_EQ(findBin("/bin/cat"), "/bin/cat");
	EXPECT_EQ(findBin("cat"), "/usr/bin/cat");
}

TEST(ShellTest, testVecConversion) {
	vector<string> test = {"aaaa", "b", "c", "d"};
	char **result = convertVector(test);

	EXPECT_EQ(string(result[0]), test[0]);
	EXPECT_EQ(string(result[3]), test[3]);
}

TEST(ShellTest, testExpansion) {
	Rash *shell = new Rash();
	vector<string> input = {"ls", "~"};
	EXPECT_EQ(shell->expand(input)[0], "ls");
	EXPECT_EQ(shell->expand(input)[1], "/home/michael");

	input = {"ls", "~/*ub"};
	EXPECT_EQ(shell->expand(input)[1], "/home/michael/pub");
	input = {"ls", "*ub"};
	EXPECT_EQ(shell->expand(input)[1], "pub");
	input = {"ls", "D*"};
	EXPECT_EQ(shell->expand(input)[2], "Documents");
	input = {"ls", "/*"};
	EXPECT_EQ(shell->expand(input)[4], "/sys");
}


GTEST_API_ int main(int argc, char *argv[]){
	printf("Running tests...\n\r");
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
