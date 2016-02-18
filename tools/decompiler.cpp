
#include <iostream>
#include <fstream>
#include "../mini_instructions.h"
#include <cstdlib>

using namespace std;

typedef unsigned char bytecode_type;

void decompile(const char* buffer, size_t len);

int main(int argc, char *argv[]) {

	if (argc == 1) {
		cerr << "error : no input files" << endl;
		exit(0);
	} else if (argc > 2) {
		cerr << "error : too more arguments" << endl;
		exit(0);
	} 

	ifstream fin;

	string addr = argv[1];
	string file_suffix = addr.substr(addr.rfind(".") + 1);

	if (file_suffix != "mo") {
		cerr << "error : the suffix of file should be \'mo\'" << endl;
		exit(0);
	}

	fin.open(addr.c_str(), ifstream::binary);
	if (!fin) {
		cerr << "error : failed to open" << endl;
		exit(0);
	}

	fin.seekg(0, fin.end);
	int len = fin.tellg();
	fin.seekg(0, fin.beg);

	char* buffer = new char[len];

	fin.read(buffer, len);

	decompile(buffer, len);

	fin.close();
	delete[] buffer;
	
	return 0;
}

void decompile(const char* buffer, size_t len) {
	unsigned char* ptr = (unsigned char*)(buffer);
	for (unsigned i = 0; i < len; i++) {
		string tmp = ministl::bytecode_to_instruction(ptr[i]);
		printf("%2d : %d %s\n", i, ptr[i], tmp.c_str());
	}
}
