// main.cpp

#include <iostream>
#include "mini_vm.h"

int main() {
	
	bytecode_type sz[] = {
		ICONST_0, ISTORE_0, BIPUSH, 100,
		ISTORE_1, ILOAD_1, JE, 0, 19, ILOAD_1, ILOAD_0,
		IADD, ISTORE_0, IINC, 1, 0xff,
		JMP, 0, 5, ILOAD_0, PRINT, EXIT};
	
	ministl::load_program(sz, sz + (sizeof(sz) / sizeof(sz[0])));
	ministl::run_vm();
	return 0;
}
