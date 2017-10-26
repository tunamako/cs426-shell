#include "gtest/gtest.h"
#include "rash.h"
#include <string.h>
#include <unistd.h> 
#include <stdio.h>

TEST(ShellTest, testCreation){
	Rash *shell = new Rash();
	EXPECT_EQ(shell->pwd, "/home/michael/Documents/cs426/shell");
	EXPECT_EQ(shell->uname, "michael");
	EXPECT_EQ(shell->pathdirs[0], "/usr/local/sbin");
	EXPECT_EQ(shell->pathdirs[2], "/usr/bin");
}


GTEST_API_ int main(int argc, char *argv[]){
	printf("Running tests...\n");
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
