#include "vm.h"

int main() {
	using namespace ministl;
	
	bytecode_type sz[] = {
		ICONST_0, ISTORE_0, BIPUSH, 100,
		ISTORE_1, ILOAD_1, JE, 0, 19, ILOAD_1, ILOAD_0,
		IADD, ISTORE_0, IINC, 1, 0xff,
		JMP, 0, 5, ILOAD_0, PRINT, EXIT};
	
	minivm vm;
	vm.load(sz, sz + (sizeof(sz) / sizeof(sz[0])));
	vm.run();
	return 0;
}