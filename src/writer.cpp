#include <iostream>
#include <cstring> 
#include <boost/filesystem.hpp>
#include "worker.h"

void Writer::ProcessInputData() {
		
	char file[max_filename_lengh];
	std::string line;
	std::string res;

	Util::getFullPath(file, WRITERS, m_inputData.c_str());
	std::ifstream writersfs(file);

	//sanitize check
	if(!writersfs.is_open()) {
		std::cout << "(Writer object) " << m_inputData << ": writer data not found" << std::endl;
		std::cout << "Please remove it's entry from writers list file or add it to writers folder" << std::endl;
		return;
	}

	//write-though policy
	while(std::getline(writersfs, line)) {
		std::string value = "";
		Util::getValueOfEntry(line, value);

		//lookup in the cache memory
		if(!line.empty()) {
			bool bCacheHit = m_lrucache->get(line, value);
			if(bCacheHit)
				m_lrucache->put(line, value);
		}
		MemoryRequest(std::atoi(line.c_str()), value);
	}

	writersfs.close();
}

void Writer::MemoryRequest(int position, std::string &value) {

	std::lock_guard<std::mutex> locker(disk_mutex);

	std::ifstream inputfs(m_memoryName);
	std::fstream outputfs("newfile.txt", std::ios::out);
	std::string line;
	size_t lineno = 0;

	//sanitize check
	if(!inputfs.is_open()) {
		std::cout << "(Writer object) memory data named: '" << m_memoryName << "' not found " << std::endl;
		std::cout << "Please copy from data folder to build folder\n";
		return;
	}

	//navigate to the entry
	while(std::getline(inputfs, line) || lineno < position) {
		++lineno;
		if(lineno == position)
			outputfs << value << std::endl;
		else
			outputfs << line << std::endl;
	}

	inputfs.close();
    outputfs.close();
   
	if (!boost::filesystem::exists("newfile.txt"))
	{
  		std::cout << "(Writer object) can't find processed file!" << std::endl;
  		return;
	}
	if(remove(m_memoryName.c_str()) != 0)
    	std::cout << "(Writer object): " << strerror(errno) << std::endl;    	
    if(rename("newfile.txt", m_memoryName.c_str()) != 0)
    	std::cout << "(Writer object): " << strerror(errno) << std::endl;  
}