// asmer.cpp

#include <iostream>
#include <vector>
#include <fstream>
#include "asmer.h"
#include "../mini_instructions.h"
#include <cstdlib>

using namespace std;

typedef unsigned char bytecode_type;

int main(int argc, char *argv[]) {
	ifstream fin;
	ofstream fout;

    string addr;

	if (argc == 1) {
		cerr << "error : no input files" << endl;
        exit(0);
        // for debug
		// addr = "C:\\Users\\wh\\Desktop\\vm\\src\\asm\\test1.ma";
	} else addr = argv[1];

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

	fin.open(addr.c_str(), ifstream::binary);
	fout.open(newaddr.c_str(), ofstream::binary);

	if (!fin) {
		cerr << "error : failed to open" << endl;
		exit(0);
	}

	if (!fout) {
		cerr << "error : failed to create a new file \"" +
		newaddr << endl;
		exit(0);
	}

	cout << "new file is " << newaddr << endl;


	fin.seekg(0, fin.end);
	size_t length = fin.tellg();
	fin.seekg(0, fin.beg);

	char * buffer = new char[length];

	fin.read(buffer, length);
	fin.close();

	ministl::miniasm myAsm;
	vector<bytecode_type> ret;
	myAsm.read(buffer, length);

	ret = myAsm.write();

	for (unsigned i = 0; i < ret.size(); i++) {
    #if __FOR_DEBUG
        cout << i << " : " << static_cast<unsigned>(ret[i]) << '\t'
            << ministl::bytecode_to_instruction(ret[i]) << endl;
    #endif
		fout << static_cast<char>(ret[i]);
	}

	delete[] buffer;
	fout.close();

	return 0;
}
