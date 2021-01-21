#ifndef READER_H
#define READER_H

#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>

#include "configuration.h"
#include "cache.h"
#include "utility.h"

#define max_filename_lengh 100

extern std::mutex disk_mutex;
extern std::string disk_memory_name;
typedef std::shared_ptr<LRUCache<std::string, std::string>> SHARED_CACHE;

/*
	Base class for Reader ans Writer 
*/
class Worker {

public:
	Worker()
	:m_memory_name(disk_memory_name) {}

	virtual ~Worker() {}

	virtual void ProcessInputData(){};
	virtual void MemoryRequest(){};

protected:
	std::string m_memory_name;
};

/*
	Reader class
*/
class Reader : public Worker {

public:
	Reader(SHARED_CACHE cache, std::string &data)
	:m_lrucache(cache), m_input_data(data) {}

	virtual ~Reader() {}

	void ProcessInputData();
	void MemoryRequest(int position, std::string &value);

private:
	std::string m_input_data;
	SHARED_CACHE m_lrucache;

};

/*
	Writer class
*/
class Writer : public Worker {

public:
	Writer(SHARED_CACHE cache, std::string &data)
	:m_lrucache(cache), m_input_data(data) {}

	virtual ~Writer() {}

	void ProcessInputData();
	void MemoryRequest(int position, std::string &vaue);

private:
	std::string m_input_data;
	SHARED_CACHE m_lrucache;
};

#endif //WORKER_H




