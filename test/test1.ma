# test1.ma

# 输出一到一百
# int i = 1;
# while (i < 100) {
#	print(i);
#	i++;
# }
#
# 等价于
#
# 	i = 1
#Loop:
# 	if i > 100 than goto End
# 	print 1
# 	++i
#	goto Loop
#End:
# 	exit

	iconst_1	# i = 1
	istore_1	# store i in local param 1
Loop:
	ldc  100	# push 100
	iload_1		# push i
	isub		# 100 - i
	jl  End		# if 100 - i < 0 than goto End
	iload_1		# push i
	print		# print i
	iinc  1, 1	# i++
	jmp  Loop	# goto 
End:
	exit		# over
