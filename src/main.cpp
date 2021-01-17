#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <unistd.h>
#include <string.h>
#include <boost/filesystem.hpp>

#include "worker.h"

typedef std::shared_ptr<LRUCache<std::string, std::string>> SHARED_CACHE;
typedef void (*ThreadFunc)(std:: string, SHARED_CACHE);
std::mutex disk_mutex;
std::string disk_memoryName;

int main(int argc, char *argv[]) {

	int cache_capacity = std::atoi(argv[1]);

	//not process if parameters are wrong or missing
	if((argc < 5) || (cache_capacity <= 0)) {
		std::cout << "Please enter valid input!\n";
		std::cout << "Example: ./LRUCache <size_of_cache> <reader_file> <writer_file> <items_file>\n"; 
		return -1;
	}

	//load readers, writers file
	char* readersFile = argv[2];
	char* writersFile = argv[3];
	char* itemsFile = argv[4];

	//store path of data folder
	char data_path[max_filename_lengh];
	Util::getFullPath(data_path, DATA, argv[4]);

	//init LRU Cache object and set disk memory file name
	SHARED_CACHE cacheObj = std::make_shared<LRUCache<std::string, std::string>>(cache_capacity);
	disk_memoryName = itemsFile;

	std::ifstream readers(readersFile, std::ios::in);
	std::ifstream writers(writersFile, std::ios::in);
	std::vector<std::thread> vThreadReader, vThreadWriters;
	std::string line;


	//sanitize check
	if(!readers.is_open()) {
		std::cout << "Input reader list not found, please copy from data folder to build folder\n";
		return -1;
	}
	if(!writers.is_open()) {
		std::cout << "Input writer list not found, please copy from data folder to build folder\n";
		return -1;
	}

	//start reader threads
	while(std::getline(readers, line)) {
		vThreadReader.push_back(std::thread(&Reader::ProcessInputData, Reader(cacheObj, line)));
	}

	//start wrters threads
	while(std::getline(writers, line)) {
		vThreadWriters.push_back(std::thread(&Writer::ProcessInputData, Writer(cacheObj, line)));
	}

    for_each(vThreadReader.begin(), vThreadReader.end(), [](std::thread &t){ t.join();});
    for_each(vThreadWriters.begin(), vThreadWriters.end(), [](std::thread &t){ t.join();});

    //cache display content
	//cacheObj->display();

	//place processed data file with original one
	char processedData [max_filename_lengh];
	try {
		Util::getFullPath(processedData, BUILD_OUTPUT, argv[4]);

		if (!boost::filesystem::exists(processedData))
		{
  			std::cout << "Can't find processed data file!" << std::endl;
  			return -1;
		}

		remove(data_path);
		boost::filesystem::copy_file(processedData, data_path);
	}
	catch (boost::filesystem::filesystem_error& e) {
		std::cout << "Error while replace data: " << std::endl;
		std::cout << e.what() << std::endl;
	}

	return 0;
}