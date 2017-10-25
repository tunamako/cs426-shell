#include "gtest/gtest.h"
#include "rash.h"
#include <string.h>
#include <unistd.h> 
#include <stdio.h>

TEST(ShellTest, testGetPath){
	Rash *shell = new Rash();
	EXPECT_EQ(shell->pwd, "/home/michael/Documents/cs426/shell");
	EXPECT_EQ(shell->PATH, "/usr/local/sbin:/usr/local/bin:/usr/bin:/usr/lib/jvm/default/bin:/usr/bin/site_perl:/usr/bin/vendor_perl:/usr/bin/core_perl");

}


GTEST_API_ int main(int argc, char *argv[]){
	printf("Running tests...\n");
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
