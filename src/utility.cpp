#include "utility.h"
#include <string.h>
#include <bits/stdc++.h>


void Util::getOutputFileName(char* outputFile, const char* fileName) {

	strncpy(outputFile, fileName, 7);
	strcat(outputFile, ".out.txt\0");
}

void Util::getValueOfEntry(std::string &entry, std::string &value) {
	std::string space = " ";

	int pos = entry.find(space);
	if(pos != std::string::npos) {
		value = std::string(entry, pos+1);
	}
}