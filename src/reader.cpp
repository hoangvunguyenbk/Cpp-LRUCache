#include <iostream>
#include <boost/filesystem.hpp>
#include "worker.h"

void Reader::ProcessInputData() {
	
	char file[max_filename_lengh];
	char output_file[max_filename_lengh];

	Util::GetFullPath(file, READERS, m_input_data.c_str());
	Util::GetFullPath(output_file, READER_OUTPUT, m_input_data.c_str());

	//format output file
	strcat(output_file, ".out");

	std::string line;
	std::ifstream readerfs(file);
	std::fstream reader_outputfs(output_file, std::ios::out);

	//sanitize check
	if(!readerfs.is_open()) {
		std::cout << "(Reader object) " << m_input_data << ": reader data not found" << std::endl;
		std::cout << "Please check the file format and the order of your command parameters.\n";
		std::cout << "If input data file not exist in readers folder, please remove it's entry from the readers list.\n\n";
		return;
	}

	//reading Reader[i].txt file line by line
	while(std::getline(readerfs, line)) {
		std::string value  = "";

		//lookup in the cache memory
		bool isCacheHit = false;
		if(!line.empty()) {
			isCacheHit = m_lrucache->Get(line, value);
			if(!isCacheHit) {
				//lookup from the disk
				MemoryRequest(std::atoi(line.c_str()), value);
				//put new value to the cache
				m_lrucache->Put(line, value);
			}
		}

		//write result to Reader[i].out.txt
		//if not found on both cache and disk, write an empty string
		std::string res = (isCacheHit) ? "Cache" : (value.empty()) ? "" : "Disk";
		reader_outputfs << value << '\t' << res << '\t' << std::endl;
	}

	readerfs.close();
	reader_outputfs.close();
}

void Reader::MemoryRequest(int position, std::string &value) {

	std::lock_guard<std::mutex> locker(disk_mutex);

	std::ifstream readerfs(m_memory_name, std::ios::in);
	std::string line;
	size_t lineno = 1;

	//sanitize check
	if(!readerfs.is_open()) {
		std::cout << "(Reader object) memory data named: '" << m_memory_name << "' not found " << std::endl;
		std::cout << "Please copy from data folder to build folder\n";
		return;
	}

	//navigate to the entry
	while(std::getline(readerfs, line) && lineno < position) {
		++lineno;
	}
	value = line;

	readerfs.close();
}