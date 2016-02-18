// test_asmer.cpp

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include "mini_asmer.h"
#include "mini_instruction_set.h"

using namespace std;

typedef unsigned char bytecode_type;

int main() {
	ifstream fin;
	fin.open("C:\\Users\\wh\\Desktop\\vm\\test.ma");
	vector<bytecode_type> re = ministl::parse(fin);
	cout << "Total " << re.size() << " lines." << endl;
	int count = 0;
	for (unsigned i = 0; i < re.size(); i++) {
		printf("%-3d : %2x\t\t", count, re[i]);
		cout << ministl::bytecode_to_instruction(re[i]) << endl;
		count++;
	}
	return 0;
}
