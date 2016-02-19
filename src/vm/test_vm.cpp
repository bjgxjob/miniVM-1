#include "vm.h"

int main() {
	using namespace ministl;
	
	/* bytecode_type sz[] = {
		ICONST_0, ISTORE_0, BIPUSH, 100,
		ISTORE_1, ILOAD_1, JE, 0, 19, ILOAD_1, ILOAD_0,
		IADD, ISTORE_0, IINC, 1, 0xff,
		JMP, 0, 5, ILOAD_0, PRINT, EXIT}; */

	bytecode_type sz[] = {
		LDC, 0x3f, 0x99, 0x99, 0x9a,
		LDC, 0x40, 0x00, 0x00, 0x00,
		FADD, FPRINT, EXIT };
	
	minivm vm;
	vm.load(sz, sz + (sizeof(sz) / sizeof(sz[0])));
	vm.run();
	return 0;
}