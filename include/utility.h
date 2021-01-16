#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <bits/stdc++.h>

class Util {
public:
	Util() {};
	virtual ~Util() {};

	/*
	  Input  : reader file name. Etc: Reader[i].txt
	  Output : output reader file name. etc: Reader[i].out.txt 
	*/
	static void getFullPath(char* file, const char* path, const char* file_name) {

		strcpy(file, path);
		strcat(file, file_name);		
	}

	/*
	  Input  : a string represent for position and value to write. Etc <entry value>
	  Output : a value of the entry 
	*/
	static void getValueOfEntry(std::string &entry, std::string &value) {
			
		std::string space = " ";
		int pos = entry.find(space);
		if(pos != std::string::npos) {
			value = std::string(entry, pos+1);
		}
	}
};

#endif //UTILITY_H