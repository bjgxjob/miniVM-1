# func.ma

	jmp _main

_func:
	sipush 999
	print
	ret

_main:
	iconst_1
	istore_1
	iload_1
	print
	iconst_2
	istore_2
	iload_2
	print
	iload_1
	iload_2
	call _func
	iadd
	print
