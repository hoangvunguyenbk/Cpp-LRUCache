#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <unistd.h>
#include <string.h>

#include "configuration.h"
#include "utility.h"
#include "cache.h"

#define max_filename_lengh 100
std::mutex disk_mutex;

typedef std::shared_ptr<LRUCache<std::string, std::string>> SHARED_CACHE;
typedef void (*ThreadFunc)(std:: string, SHARED_CACHE);

void writeToDisk(std::string fileName, std::string &entry, std::string &value) {
	
	std::lock_guard<std::mutex> locker(disk_mutex);

	std::fstream inputfs(fileName, std::ios::in);
	std::fstream outputfs("newfile.txt", std::ios::out);
	std::string line;
	size_t lineno = 0;

	//Navigate to the entry
	while(std::getline(inputfs, line) || lineno < std::atoi(entry.c_str())) {
		++lineno;
		if(lineno == std::atoi(entry.c_str()))
			outputfs << value << std::endl;
		else
			outputfs << line << std::endl;
	}

	inputfs.close();
    outputfs.close();

    remove(fileName.c_str());
    rename("newfile.txt", fileName.c_str());
}

void diskLookup(std::string fileName, std::string &entry, std::string &value) {

	std::lock_guard<std::mutex> locker(disk_mutex);

	std::fstream readerfs(fileName, std::ios::in);
	std::string line;
	size_t lineno = 1;

	//Navigate to the entry
	while(std::getline(readerfs, line) && lineno < std::atoi(entry.c_str())) {
		++lineno;
	}
	value = line;

	readerfs.close();
}

void readerThread(std::string fileName, SHARED_CACHE cache) {

	char file[max_filename_lengh];
	char output_file[max_filename_lengh];

	Util::getFullPath(file, READERS, fileName.c_str());
	Util::getFullPath(output_file, READER_OUTPUT, fileName.c_str());

	std::string line;
	std::fstream readerfs(file, std::ios::in);
	std::fstream reader_outputfs(output_file, std::ios::out);

	//Reading Reader[i].txt file line by line
	while(std::getline(readerfs, line)) {
		std::string value  = "";

		//Lookup in the cache memory
		bool bCacheHit = false;
		if(!line.empty()) {
			bCacheHit = cache->get(line, value);
			if(!bCacheHit) {
				//Lookup from the disk
				diskLookup("Items", line, value);
				//put new value to the cache
				cache->put(line, value);
			}
		}

		//Write result to Reader[i].out.txt
		//If not found on both cache or disk, write an empty string
		std::string res = (bCacheHit) ? "Cache" : (value.empty()) ? "" : "Disk";
		reader_outputfs << value << '\t' << res << '\t' << std::endl;
	}

	readerfs.close();
	reader_outputfs.close();
}

void writerThread(std::string fileName, SHARED_CACHE cache) {

	char file[max_filename_lengh];
	std::string line;
	std::string res;

	Util::getFullPath(file, WRITERS, fileName.c_str());
	std::ifstream writerst(file);

	//Write-though policy
	while(std::getline(writerst, line)) {
		std::string value = "";
		Util::getValueOfEntry(line, value);

		//Lookup in the cache memory
		if(!line.empty()) {
			bool bCacheHit = cache->get(line, value);
			if(bCacheHit)
				cache->put(line, value);
		}
		writeToDisk("Items", line, value);
	}

	writerst.close();
}

int main(int argc, char *argv[]) {

	//return if cache size <= 0
	if((argc < 5) || (argv[1] <= 0)) {
		std::cout << "please give input!\n";
		return 0;
	}

	//load readers, writers file
	int cache_capacity = std::atoi(argv[1]);
	char* readersFile = argv[2];
	char* writersFile = argv[3];
	char* itemsFile = argv[4];

	//Init LRU Cache object
	SHARED_CACHE cacheObj = std::make_shared<LRUCache<std::string, std::string>>(cache_capacity);

	std::fstream readers(readersFile, std::ios::in);
	std::fstream writers(writersFile, std::ios::in);
	std::vector<std::thread> vThreadReader, vThreadWriters;
	std::string line;

	while(std::getline(readers, line)) {
		ThreadFunc t = &readerThread;
		vThreadReader.push_back(std::thread(t,line , cacheObj));
	}

	while(std::getline(writers, line)) {
		ThreadFunc t = &writerThread;
		vThreadWriters.push_back(std::thread(t,line, cacheObj));
	}

	for_each(vThreadReader.begin(), vThreadReader.end(), [](std::thread &t){ t.join();});
	for_each(vThreadWriters.begin(), vThreadWriters.end(), [](std::thread &t){ t.join();});

	cacheObj->display();

	return 0;
}