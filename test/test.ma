# test.ma
.limit stack 4		# size of stack is 4
.limit locals 8		# size of locals param array is 8

	iconst_0			# sum
	istore_0 			# 将sum = 0存到local_0
	ldc  100 			# i
	istore_1 			# 将i = 100存到local_1
Loop:
	iload_1 			# 取出i
	je  End				# 栈顶元素为零则跳转
	iload_1				# 取出i
	iload_0  			# 加载sum
	iadd 				# 相加
	istore_0 			# 更新sum值
	iinc  1, -1			# --i
	jmp  Loop 			# 循环
End:
	iload_0 			# sum待输出
	iprint 				# print to stdout
	exit

