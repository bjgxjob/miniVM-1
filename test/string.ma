# string.ma

.string str1 "hello \nworld!\n"
.string str2 "\nhello miniASM!\n"
.global global_var 65535
.string str3 "You can say :\"hey man! your name is \x41\x54!\"\n"

	puts  str1		# str1 is the address of the first char
	iglobal global_var
	iprint   		# the type of global_var is int
	puts  str2
	puts  str3