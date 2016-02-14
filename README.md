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


