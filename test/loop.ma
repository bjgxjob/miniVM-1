# loop.ma

_main:
	ldc  1234567
	call  _loop
	exit

_loop:
	istore_1		# save n
Loop:
	iload_1
	iconst_1
	isub			# n - 1
	dup
	istore_1
	jne Loop
	ret

