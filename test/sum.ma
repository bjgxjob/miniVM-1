# sum.ma


_main:
	iconst_5
	iconst_4
	call _sum
	iprint
	exit

_sum:
	istore_1
	istore_2
	iload_1
	iload_2
	iadd
	ret