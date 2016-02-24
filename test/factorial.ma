# factorial.ma

_main:
	bipush  15
	call  _func
	iprint
	exit


_func:		
	istore_1

	iload_1
	iconst_1
	isub
	jne Rec
	iconst_1
	ret

Rec:
	iload_1
	iconst_1
	isub
	call _func
	istore_2
	iload_2
	iload_1
	imul
	ret
