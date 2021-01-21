	#include <iostream>
#include <boost/filesystem.hpp>
#include "gtest/gtest.h"
#include "worker.h"
#include "configuration.h"
#include "utility.h"

using namespace std;

enum MODE
{
	READER,
	WRITER
};

std::mutex disk_mutex;
std::string disk_memory_name;

class WorkerTest : public ::testing::Test {

protected:

	WorkerTest() {}
	virtual ~WorkerTest() {}

	Reader* reader_obj = nullptr;
	Writer* writer_obj = nullptr;
	SHARED_CACHE cache;

	char test_data_new[max_filename_lengh];
	char data_memory[max_filename_lengh];
	std::string data_memory_obj;

	virtual void SetUp(MODE mode, char* data_memory, const char* test_data) {

		Util::GetFullPath(data_memory, UNITTEST, "TEST_DATA");
		Util::GetFullPath(test_data_new, UNITTEST, test_data);

		cache = std::make_shared<LRUCache<std::string, std::string>> (3);
		disk_memory_name = test_data_new;

		reader_obj = new Reader(cache, disk_memory_name);
		writer_obj = new Writer(cache, disk_memory_name);

		//clone a copy of original test data, each test case will have it own data base
		boost::filesystem::copy_file(data_memory, test_data_new);
	}

	virtual void TearDown() {

		remove(test_data_new);
		delete reader_obj;
		delete writer_obj;
	}
};

    /*
	ORIGINAL CONTENT OF TEST_DATA
	---------------------------
	pos |       value
	---------------------------
	1	 		1000
	2			2000	
	3			3000
	4			4000
	5			5000	
	---------------------------
    */

TEST_F(WorkerTest, Reader_MemoryRequest) {

	SetUp(MODE::READER, data_memory, "TEST_DATA_NEW1");
	
	std::string value = "";

	if(reader_obj)
		reader_obj->MemoryRequest(1, value);
	/*
	---------------------------
	pos |       value
	---------------------------
	1	 		1000
	2			2000	
	3			3000
	4			4000
	5			5000	
	---------------------------
    */
	ASSERT_EQ(value, "1000");

}

TEST_F(WorkerTest, Writer_MemoryRequest) {

	SetUp(MODE::READER, data_memory, "TEST_DATA_NEW2");

	string write_value = "ABCD";
	string read_value = "";

	if(writer_obj)
		writer_obj->MemoryRequest(1, write_value);

	if(reader_obj)
		reader_obj->MemoryRequest(1, read_value);
    /*
	---------------------------
	pos |       value
	---------------------------
	1	 		ABCD
	2			2000	
	3			3000
	4			4000
	5			5000	
	6			6000
	7			7000
	---------------------------
    */
	ASSERT_EQ(write_value, read_value);

}


