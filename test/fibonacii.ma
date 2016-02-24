# fibonacii.ma

_main:
	bipush  30
	call _fib1
	iprint			# print the result
	bipush 	10
	cprint			# print '\n'
	exit



_fib1:	

	istore_1		# save the argument n

	# if n - 1 > 0
	iload_1			# push n
	iconst_1		# push 1
	isub			# n - 1
	jg Recur		# if n > 1 goto Recur
	# else 
	iconst_1		# push 1
	ret 			# return 1

Recur:

	# fib1(n - 1)
	iload_1			# push n
	iconst_1		# push 1
	isub			# pass n - 1 to _fib1
	call _fib1		
	istore_2 		# the return value
	# print return value

	# fib1(n - 2)
	iload_1
	iconst_2
	isub
	call _fib1

	# sum and return
	iload_2
	iadd
	ret




_fib2:
	exit