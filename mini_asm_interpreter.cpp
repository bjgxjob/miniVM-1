// miniASM Interpreter

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "mini_asmer.h"
#include "mini_instruction_set.h"
#include "mini_vm.h"


using namespace std;

typedef unsigned char bytecode_type;

int main(int argc, char *argv[]) {

	ifstream fin;

	if (argc == 1) {
		cerr << "error : no input files" << endl;
		exit(0);
	}

	if (!fin) {
		cerr << "error : failed to open" << endl;
		exit(0);
	}

	string addr = argv[1];
	fin.open(addr);

	vector<bytecode_type> prog = ministl::parse(fin);

	ministl::load_program(prog.begin(), prog.end());
	ministl::run_vm();

	fin.close();
	return 0;
}