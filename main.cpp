// main.cpp

#include <iostream>
#include "mini_vm.h"

int main() {
    using namespace ministl;
	bytecode_type sz[] = {NOP, ICONST_0, ISTORE_0, JMP, 0, 14, JMP, 0, 20,
		ILOAD_0, PRINT, IINC, 0, 1, ILOAD_0, BIPUSH, 100, JL, 0, 9, EXIT};
	load_program(sz, sz + (sizeof(sz) / sizeof(sz[0])));
	run_vm();
	return 0;
}
