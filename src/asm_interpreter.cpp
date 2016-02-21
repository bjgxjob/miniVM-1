// miniASM Interpreter

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "asm/asmer.h"
#include "mini_instructions.h"
#include "vm/vm.h"


using namespace std;

typedef unsigned char bytecode_type;

int main(int argc, char *argv[]) {

	ifstream fin;

	if (argc == 1) {
		cerr << "error : no input files" << endl;
		exit(0);
	}

	string addr = argv[1];
	fin.open(addr.c_str(), ifstream::binary);

	if (!fin) {
		cerr << "error : failed to open" << endl;
		exit(0);
	}

	fin.seekg(0, fin.end);
	size_t length = fin.tellg();
	fin.seekg(0, fin.beg);

	char * buffer = new char[length];
	fin.read(buffer, length);
	fin.close();

	ministl::miniasm myAsm;
	ministl::minivm vm;

	myAsm.read(buffer, length);
	vector<bytecode_type> prog = myAsm.write();
	
	vm.load(prog.begin(), prog.end());


	clock_t first = clock();
	vm.run();		// run	
	clock_t last = clock();
	double time_waste = static_cast<double>(last - first) / CLOCKS_PER_SEC;
	
	printf("\nProcess execution time : %.3lfs\n", time_waste);
	system("pause");
	
	return 0;
}