// mini_vm.cpp

#include <iostream>
#include <fstream>
#include "mini_vm.h"
#include <cstdlib>

using namespace std;

typedef unsigned char bytecode_type;

int main(int argc, char *argv[]) {

	ifstream fin;

	if (argc == 1) {
		cerr << "error : no input files" << endl;
		exit(0);
	}

	string addr = argv[1];

	string file_suffix = addr.substr(addr.rfind(".") + 1);

	if (file_suffix != "mo") {
		cerr << "error : the suffix of file should be \'mo\'" << endl;
		exit(0);
	}

	fin.open(addr, ifstream::binary);

	if (!fin) {
		cerr << "error : failed to open" << endl;
		exit(0);
	}

	fin.seekg(0, fin.end);
	int len = fin.tellg();
	fin.seekg(0, fin.beg);

	char* buffer = new char[len];

	fin.read(buffer, len);

	ministl::load_program(buffer, buffer + len);

	fin.close();
	delete[] buffer;

	ministl::run_vm();
	
	return 0;
}
