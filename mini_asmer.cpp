// miniASMer

#include <iostream>
#include <vector>
#include <fstream>
#include "mini_asmer.h"
#include "mini_instruction_set.h"
#include <cstdlib>

using namespace std;

typedef unsigned char bytecode_type;

int main(int argc, char *argv[]) {
	ifstream fin;
	ofstream fout;

	if (argc == 1) {
		cerr << "error : no input files" << endl;
		exit(0);
	}

	string addr = argv[1];

	string file_suffix = addr.substr(addr.rfind(".") + 1);

	if (file_suffix != "ma") {
		cerr << "error : the suffix of file should be \'ma\'" << endl;
		exit(0);
	}

	string name_newfile = "a";
	if (argc == 3) {
		name_newfile = argv[2];
	}

	string newaddr = addr.substr(0, addr.rfind("\\") + 1) + 
						name_newfile + ".mo";
	
	fin.open(addr);
	fout.open(newaddr);

	if (!fin) {
		cerr << "error : failed to open" << endl;
		exit(0);
	}

	if (!fout) {
		cerr << "error : failed to create a new file \"" + 
		newaddr << endl;
		exit(0);
	}


	vector<bytecode_type> re = ministl::parse(fin);
	cout << "Total " << re.size() << " bytes." << endl;
	cout << "new file is " << newaddr << endl;
	for (unsigned i = 0; i < re.size(); i++) {
		fout << static_cast<char>(re[i]);
	}
	
	fin.close();
	fout.close();
	return 0;
}