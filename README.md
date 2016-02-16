# 一个简单的栈式虚拟机实现 —— miniVM

*注：为了简化流程 全程中文*

* 目的：为之后练习写编译器提供字节码和对应的虚拟机，所以这次实现可复用，可更新。

* 主要内容：用C++实现，尽量简洁，可扩展。

* 输入：设计好的字节码（单字节），即以unsigned char实现。

* 输出：运行字节码后的结果，显示结果到屏幕上，写文件（待添加）等。

* 实现注意点如下：
	从最简化（以至于不能再简化）的情况开始，先把整个框架写出来，不考虑任何效率问题，之后再作函数替换。

* 字节码目前有：NOP, ADD, SUB, MUL, DIV,
	ISTORE_0, ISTORE_1, ISTORE_2, ISTORE_3,
	ILOAD_0, ILOAD_1, ILOAD_2, ILOAD_3,
	ICONST_0, ICONST_1, ICONST_2, ICONST_3, ICONST_4, ICONST_5,
	BIPUSH, IINC,
	PRINT, JMP, JEZ, JL, IF, READLINE, RET, CALL, EXIT。

---

# 一个配套的汇编器 —— miniASMer

* A mini assembler for miniVM。

* 特点：小而精简。

* 目的：将特定的汇编语言（miniasm）转化成miniVM的机器码。

* 先确定miniasm的语法：
	miniasm语句分为三类，命令（Instruction），汇编指令（Directive），标号（Label）。
	1、命令，即指令，包括标号、助记符和参数（可能为标号）。
	2、以小数点dot为开头即表示汇编指令（如.limit stack 4表示栈的容量设定为4）。
	3、注释用#号表一行注释（note），无多行注释。
	4、以上三种类型都单独作为一行使用，可以任意次序搭配，注释可以在任何地方。

* 一个简单的例子：

```miniasm
# 编写一个从一加到一百的程序(result == 5050)

# int i = 100, sum = 0;
# while (i != 0) {
#	sum += i;
# }
# print(sum);

# 等价于

# i = 100
# sum = 0
#Loop:
# if i != 0 goto End
# sum += i
# goto Loop
#End:
# print sum
# exit

.limit stack 4		# size of stack is 4
.limit locals 8		# size of locals param array is 8

	iconst_0			# sum
	istore_0 			# 将sum = 0存到local_0
	ldc  100 			# i
	istore_1 			# 将i = 100存到local_1
Loop:
	iload_1 			# 取出i
	jnz  End			# 栈顶元素不为零则跳转
	iload_0  			# 加载sum
	iadd 				# 相加
	istore_0 			# 更新sum值
	jmp  Loop 			# 循环
End:
	iload_0 			# sum待输出
	print 				# 输出到屏幕上
	exit 				# 结束进程

```

	

