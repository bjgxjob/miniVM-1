# string.ma

.string str1 "hello \nworld!"
.string str2 "hello miniASM!"
#.global global_var 65535

	puts  str1		# str1 is the address of the first char
	exit
	#iprint  global_var 	# the type of global_var is int