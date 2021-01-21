#include <iostream>
#include "gtest/gtest.h"
#include "utility.h"

#define max_leng_file 100

using namespace std;

class UtilTest : public ::testing::Test {

protected:

	char file[max_leng_file];
	const char* path = "/home/user/workspace/";
	const char* file_name = "file_name";

	string entry = string("10 1111111");

	virtual void SetUp() {}
	virtual void TearDown() {}
};


TEST_F(UtilTest, GetFullPath) {

	Util::GetFullPath(file, path, file_name);
	std::string str = "/home/user/workspace/file_name";

	for(int i = 0; i < str.size(); ++i){
		ASSERT_EQ(str[i], file[i]);
	}
}

TEST_F(UtilTest, GetValueOfEntry) {

	string value;
	Util::GetValueOfEntry(entry, value);
	ASSERT_EQ(value, "1111111");
}


