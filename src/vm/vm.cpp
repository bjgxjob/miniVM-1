// vm.cpp
// Done.

#include <iostream>
#include <fstream>
#include "vm.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

typedef unsigned char bytecode_type;

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

	ministl::minivm vm;		// create instance

	vm.load(buffer, buffer + len);		// load the program

	fin.close();
	delete[] buffer;

	clock_t first = clock();

	vm.run();		// run
	
	clock_t last = clock();
	double time_waste = static_cast<double>(last - first) / CLOCKS_PER_SEC;
	
	printf("\nProcess execution time : %.3lfs\n", time_waste);
	
#ifdef _WIN32
	system("pause");
#elif _WIN64
	system("pause");
#elif linux
	/* do nothing */;
#else
	cout << "Press enter to continue..." << endl;
	getchar();
#endif

	return 0;
}

