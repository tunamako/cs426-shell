#include "gtest/gtest.h"
#include "rash.h"
#include "operator.h"
#include "helpers.h"

using namespace std;
TEST(HelpersTest, testBinSearch) {
	EXPECT_EQ(findBin("/bin/ls"), "/bin/ls");
	EXPECT_EQ(findBin("ls"), "/usr/bin/ls");
	EXPECT_EQ(findBin("/bin/cat"), "/bin/cat");
	EXPECT_EQ(findBin("cat"), "/usr/bin/cat");
}

TEST(HelpersTest, testGetOperatorPos) {
	string operators = "|<>";
	vector<string> input = {"ls", "~/pub", "|", "grep", "wah"};
	EXPECT_EQ(getLastPositionOf(input, operators), 2);
	input = {"ls","<","test"};
	EXPECT_EQ(getLastPositionOf(input, operators), 1);
	input = {"ls", ">", "test"};
	EXPECT_EQ(getLastPositionOf(input, operators), 1);

}

TEST(HelpersTest, testVecConversion) {
	vector<string> test = {"aaaa", "b", "c", "d"};
	char **result = convertVector(test);

	EXPECT_EQ(string(result[0]), test[0]);
	EXPECT_EQ(string(result[3]), test[3]);
}



TEST(ShellTest, testCreation){
	Rash *shell = new Rash();

	EXPECT_EQ(shell->uname, "michael");
	EXPECT_EQ(shell->pathdirs[0], "/usr/local/sbin");
	EXPECT_EQ(shell->pathdirs[2], "/usr/bin");

	Rash *anothershell = new Rash();
	EXPECT_EQ(anothershell->uname, "michael");
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
}

TEST(ShellTest, testInputParsing) {
	Rash *shell = new Rash();
	vector<string> input = {"ls", "~/pub", "|", "grep", "wah"};

	Op *root = shell->parse(input);
	EXPECT_EQ(root->type(), "PipeOp");
	EXPECT_EQ(root->lhs->type(), "CommandOp");
	EXPECT_EQ(root->lhs->lhs->type(), "NullOp");
	delete root;

	input = {"ls", "~/pub", ">", "test", "|", "grep", "wah"};

	root = shell->parse(input);
	EXPECT_EQ(root->type(), "PipeOp");
	EXPECT_EQ(root->lhs->type(), "OutputRedirOp");
	delete root;
	input = {"ls", "~/pub", ">", "test", "|", "grep", "<", "test2"};

	root = shell->parse(input);
	EXPECT_EQ(root->type(), "PipeOp");
	EXPECT_EQ(root->lhs->type(), "OutputRedirOp");
	EXPECT_EQ(root->rhs->type(), "InputRedirOp");
	delete root;
	input = {"cat", "<", "filename", "|", "sort", ">", "sortedFile"};

	root = shell->parse(input);
	EXPECT_EQ(root->type(), "PipeOp");
	EXPECT_EQ(root->lhs->type(), "InputRedirOp");
	EXPECT_EQ(root->rhs->type(), "OutputRedirOp");
	delete root;

	input = {"cat", "<", "filename", "|", "grep", "wah", "|", "sort", ">", "sortedFile"};

	root = shell->parse(input);
	EXPECT_EQ(root->type(), "PipeOp");
	EXPECT_EQ(root->lhs->type(), "PipeOp");
	EXPECT_EQ(root->lhs->rhs->type(), "CommandOp");
	EXPECT_EQ(root->lhs->lhs->type(), "InputRedirOp");
	delete root;
}

GTEST_API_ int main(int argc, char *argv[]){
	printf("Running tests...\n\r");
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}