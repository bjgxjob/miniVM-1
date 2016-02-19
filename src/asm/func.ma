# func.ma

	jmp _main

_func:
	sipush 999
	iprint
	ret

_main:
	iconst_1
	istore_1
	iload_1
	iprint
	iconst_2
	istore_2
	iload_2
	iprint
	iload_1
	iload_2
	call _func
	iadd
	iprint
