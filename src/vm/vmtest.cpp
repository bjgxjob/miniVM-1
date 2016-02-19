// vm.cpp
// Done.

#include <iostream>
#include <fstream>
#include "vm.h"
#include <cstdlib>

using namespace std;

typedef unsigned char bytecode_type;

int main(int argc, char *argv[]) {

    string addr;

	if (argc == 1) {
		// cerr << "error : no input files" << endl;
		// exit(0);
		addr = "C:\\Users\\wh\\Desktop\\vm\\src\\asm\\func.mo";
	} else if (argc > 2) {
		cerr << "error : too more arguments" << endl;
		exit(0);
	} else {
        addr = argv[1];
    }

	ifstream fin;


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

	ministl::minivm vm;		// create instance

	vm.load(buffer, buffer + len);		// load the program

	fin.close();
	delete[] buffer;

	vm.run();		// run

	return 0;
}

