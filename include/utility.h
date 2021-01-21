#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <bits/stdc++.h>

class Util {

public:
	/*
	  Input  : reader file name. Etc: Reader[i].txt
	  Output : absolute path of the file on filesystem. etc: /home/user/output/Reader[i].out.txt 
	*/
	static void GetFullPath(char* file, const char* path, const char* file_name) {
		strcpy(file, path);
		strcat(file, file_name);
	}

	/*
	  Input  : a string represent for position and value to write. Etc <entry value>
	  Output : a string represent for value of the entry 
	*/
	static void GetValueOfEntry(std::string &entry, std::string &value) {
		
		std::string space = " ";
		int pos = entry.find(space);
		if(pos != std::string::npos) {
		value = std::string(entry, pos+1);
		}
	}
};

#endif //UTILITY_H