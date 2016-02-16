// main.cpp

#include <iostream>
#include "mini_vm.h"

int main() {
    using namespace ministl;
	// bytecode_type sz[] = {NOP, ICONST_0, ISTORE_0, JMP, 0, 14, JMP, 0, 20,
	//	ILOAD_0, PRINT, IINC, 0, 1, ILOAD_0, BIPUSH, 100, JL, 0, 9, EXIT};
	#if 1
	bytecode_type sz[] = {
		ICONST_0, ISTORE_0, BIPUSH, 100,
		ISTORE_1, ILOAD_1, JE, 0, 19, ILOAD_1, ILOAD_0,
		IADD, ISTORE_0, IINC, 1, 0xff,
		JMP, 0, 5, ILOAD_0, PRINT, EXIT};
	#endif
	#if 0
	bytecode_type sz[] = {
		ICONST_2, ISTORE_0, BIPUSH, 100, IINC, 0, 0xff, ILOAD_0,
		IADD, PRINT, EXIT};
    #endif
	load_program(sz, sz + (sizeof(sz) / sizeof(sz[0])));
	run_vm();
	return 0;
}
