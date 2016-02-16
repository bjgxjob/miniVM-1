// mini_vm.h

#ifndef __MINI_VM_H
#define __MINI_VM_H

#include <iostream>		// for string
						// for cout, endl
#include <stack>		// for stack
#include <vector>		// for vector
#include <climits>		// for CHAR_MAX
#include <cstdlib>		// for itoa
#include <cstdio>		// for sprintf
// #include <string>		// for substr
#include <cstddef>

#include "mini_instruction_set.h"

namespace ministl {
/*
	先想好接口
	我要的是字节码 也就是接口是unsigned char
	然后也要定义字节码
	然后将栈式计算机的各部分都对应上
	然后就可以开始连起来了
*/
using std::string;
using std::cerr;
using std::cout;
using std::endl;

using std::vector;
using std::stack;

/* CONSTANT DEFINITION */
#define __FOR_DEBUG 0

#define LOCAL_PARAM_ARRAY_SIZE 8
#define OPERAND_STACK_SIZE 4
#define PROGRAM_STACK_SIZE UCHAR_MAX


/* type definition */
enum error_type {
	WELL = 0, UNKNOWN
};

#if 0
enum {
	NOP = 0, IADD = 128, ISUB, IMUL, IDIV,
	ISTORE_0, ISTORE_1, ISTORE_2, ISTORE_3,
	ILOAD_0, ILOAD_1, ILOAD_2, ILOAD_3,
	ICONST_0, ICONST_1, ICONST_2, ICONST_3, ICONST_4, ICONST_5,
	BIPUSH, IINC,
	PRINT, JMP, JEZ, JL, IF, READLINE, RET, CALL, EXIT
};
#endif

typedef unsigned char bytecode_type;
typedef uint8_t unit_t;
typedef int32_t sys_type;	// 32位系统 cpu能运行32位数

/* cpu component (extend variables) */

static vector<bytecode_type> ByteCode_List;		// 储存程序字节码

static vector<bytecode_type> Instruction_List;		// 程序指令存放地点
static sys_type PC_Register;		// 程序计数器PC
static bytecode_type IR_Register;		// 指令寄存器
static vector<unit_t> Program_Stack(PROGRAM_STACK_SIZE);	// 程序栈 默认大小为256
static vector<sys_type> Local_Param_Array(LOCAL_PARAM_ARRAY_SIZE); 	// 局部变量区 默认大小为8
static vector<sys_type> Operand_Stack(OPERAND_STACK_SIZE);	// 操作数栈

static bool exit_flag;	// 退出循环结束进程


// ------------------------- API ----------------------------

void fetch_decode_execute_cycle();

template <typename InputIterator>
void load_program(InputIterator first, InputIterator last) {
	ByteCode_List.assign(first, last);
}

void run_vm() {
	fetch_decode_execute_cycle();
}


// -------------------------- 错误处理 ------------------------------

// 错误处理
void error_handler(error_type _err) {
	switch (_err) {
	case UNKNOWN:
		cerr << "[Error_handler] : UNKNOWN ERROR" << endl;
		break;
		// TODO();
	case WELL:
	default:
		// unhandle
		break;
	}
}

// 报错函数
void throw_error(const string& _err_str) {
	cerr << "[Error] : " << _err_str << endl;
}





// -------------------------- 部件操作实现 --------------------------

// 求值栈操作
sys_type pop_operand() {
	if (Operand_Stack.empty()) {
		throw_error("pop_operand() can\'t pop element from an empty stack.");
	}
	sys_type _ret = Operand_Stack.back();
	Operand_Stack.pop_back();
	return _ret;
}

void push_operand(sys_type _val) {
	Operand_Stack.push_back(_val);
}

string int_to_string(const int& _val) {
	char _tmp[64];
	sprintf(_tmp, "%2x", _val);
	return string(_tmp);
}

void set_variable(size_t _n, sys_type _val) {
	// 默认不越界
	Local_Param_Array[_n] = _val;
}

sys_type get_variable(size_t _n) {
	// 默认不越界
	return Local_Param_Array[_n];
}

inline bytecode_type get_next_Instruction() {
	return Instruction_List[PC_Register++];
}

inline int16_t get_next_two_Instruction() {
	int16_t _val1 = static_cast<int16_t>(get_next_Instruction());
	int16_t _val2 = static_cast<int16_t>(get_next_Instruction());
	_val2 |= (_val1 << 8);
	return _val2;
}

inline long long get_next_n_instruction(unsigned _n) {
	long long _ret = 0;
	for (unsigned i = 0; i < _n && i < sizeof(_ret); ++i) {
		_ret <<= (i * 8);
		_ret += static_cast<long long>(get_next_Instruction());
	}
	return _ret;
}

void show_stack_info() {
	cout << "-------------------- Show Info -------------------" << endl;
	cout << "\nInstruction_List => " << endl;
	for (unsigned i = 0; i < Instruction_List.size(); ++i) {
		cout << i << " : " << int_to_string(Instruction_List[i]) << endl;
	}

	cout << "\nProgram_Stack => " << endl;
	for (unsigned i = 0; i < Program_Stack.size(); ++i) {
		cout << i << " : " << int(Program_Stack[i]) << endl;
	}

	cout << "\nLocal_Param_Array => " << endl;
	for (unsigned i = 0; i < Local_Param_Array.size(); ++i) {
		cout << i << " : " << int(Local_Param_Array[i]) << endl;
	}

	cout << "\nOperand_Stack => " << endl;
	for (unsigned i = 0; i < Operand_Stack.size(); ++i) {
		cout << i << " : " << int(Operand_Stack[i]) << endl;
	}

	cout << "\nPC_Register = " << (int)PC_Register << endl;
	cout << "\nIR_Register = " << int_to_string(IR_Register) << endl;
	cout << "-------------------- END Show -------------------" << endl;
}


// ------------------------ 指令操作 ----------------------------

void opt_NOP() {
	/* do nothing */
}

void opt_ISTORE_0() {
	sys_type _val = pop_operand();
	set_variable(0, _val);
}

void opt_ISTORE_1() {
	sys_type _val = pop_operand();
	set_variable(1, _val);
}

void opt_ISTORE_2() {
	sys_type _val = pop_operand();
	set_variable(2, _val);
}

void opt_ISTORE_3() {
	sys_type _val = pop_operand();
	set_variable(3, _val);
}

void opt_ICONST_0() {
	push_operand(0);
}

void opt_ICONST_1() {
	push_operand(1);
}

void opt_ICONST_2() {
	push_operand(2);
}

void opt_ICONST_3() {
	push_operand(3);
}

void opt_ICONST_4() {
	push_operand(4);
}

void opt_ICONST_5() {
	push_operand(5);
}

void opt_ILOAD_0() {
	push_operand(get_variable(0));
}

void opt_ILOAD_1() {
	push_operand(get_variable(1));
}

void opt_ILOAD_2() {
	push_operand(get_variable(2));
}

void opt_ILOAD_3() {
	push_operand(get_variable(3));
}

void opt_IADD() {
	// 从求值栈中取出俩数相加并将结果放回栈中
	sys_type _val1 = pop_operand();
	sys_type _val2 = pop_operand();
	_val2 += _val1;
	push_operand(_val2);
}

void opt_ISUB() {
	// 从求值栈中取出俩数相减并将结果放回栈中
	sys_type _val1 = pop_operand();
	sys_type _val2 = pop_operand();
	_val2 -= _val1;
	push_operand(_val2);
}

void opt_IMUL() {
	// 从求值栈中取出俩数相乘并将结果放回栈中
	sys_type _val1 = pop_operand();
	sys_type _val2 = pop_operand();
	_val2 *= _val1;
	push_operand(_val2);
}

void opt_IDIV() {
	// 从求值栈中取出俩数相除并将结果放回栈中
	sys_type _val1 = pop_operand();
	sys_type _val2 = pop_operand();
	_val2 /= _val1;
	push_operand(_val2);
}

void opt_PRINT() {
	// 显示求值栈顶的元素
	sys_type _val = pop_operand();
	push_operand(_val);
	cout << _val;
}

void opt_EXIT() {
	// 结束循环 终止程序
	exit_flag = true;
}

void opt_JMP() {
	// 无条件跳转
	sys_type PC_n = static_cast<sys_type>(get_next_two_Instruction());
	PC_Register = PC_n;
}

void opt_JE() {
	// 等于零即跳转
	sys_type PC_n = static_cast<sys_type>(get_next_two_Instruction());
	sys_type _val = pop_operand();

	if (_val == 0) {
		PC_Register = PC_n;
	}
}

void opt_JNE() {
	// 不等于零即跳转
	sys_type PC_n = static_cast<sys_type>(get_next_two_Instruction());
	sys_type _val = pop_operand();

	if (_val != 0) {
		PC_Register = PC_n;
	}
}

void opt_JL() {
	// 不等于零即跳转
	sys_type PC_n = static_cast<sys_type>(get_next_two_Instruction());
	sys_type _val = pop_operand();

	if (_val < 0) {
		PC_Register = PC_n;
	}
}

void opt_JG() {
	// 不等于零即跳转
	sys_type PC_n = static_cast<sys_type>(get_next_two_Instruction());
	sys_type _val = pop_operand();

	if (_val > 0) {
		PC_Register = PC_n;
	}
}

#if 0
void opt_JL() {
	// 负数即跳转
	sys_type PC_n = static_cast<sys_type>(get_next_two_Instruction());
	sys_type _val1 = pop_operand();
	sys_type _val2 = pop_operand();
	if (_val2 < _val1) {
		PC_Register = PC_n;
	}
}
#endif

void opt_BIPUSH() {
	// 将后一条指令压入求值栈中
	uint8_t _tmp = get_next_Instruction();
	push_operand(static_cast<sys_type>(_tmp));
}

void opt_SIPUSH() {
	// 将后两条指令压入求值栈中
	push_operand(static_cast<sys_type>(get_next_n_instruction(2)));
}

void opt_LDC() {
	// 将后四条指令压入求值栈中
	push_operand(static_cast<sys_type>(get_next_n_instruction(4)));
}

void opt_IINC() {
	// 自增 取后两个字节码作为两个操作数
	int8_t _val1 = get_next_Instruction();
	int8_t _val2 = get_next_Instruction();
	size_t _n = static_cast<size_t>(_val1);
	sys_type _val = get_variable(_n);
	_val += _val2;
	set_variable(_n, static_cast<sys_type>(_val));
}


// ------------------------ 指令周期 -----------------------------

// 初始化循环 进入指令周期前必须初始化
// 对各个部件 如IR PC 求值栈等进行初始化
void initiating_cycle() {
	Instruction_List = ByteCode_List;	// 加载程序
	PC_Register = 0;	// 默认从零处开始
	IR_Register = NOP;		// 默认无操作
	Program_Stack.clear();
	Local_Param_Array.clear();
	Operand_Stack.clear();

	exit_flag = false;
}

// 指令读取与跳转（程序计数器）
void fetch_instruction() {
	IR_Register = Instruction_List[PC_Register];	// 获取指令
	++PC_Register;	// 指向下一条指令
}

// 解码指令
void decode_instruction() {
	/* do nothing */
}

// 运行指令 之后用map？
void execute_instruction() {
	switch (IR_Register) {
	case NOP:
		opt_NOP(); break;
	case IADD:
		opt_IADD(); break;
	case ISUB:
		opt_ISUB(); break;
	case IMUL:
		opt_IMUL(); break;
	case IDIV:
		opt_IDIV(); break;
	case PRINT:
		opt_PRINT(); break;
	case ISTORE_0:
		opt_ISTORE_0(); break;
	case ISTORE_1:
		opt_ISTORE_1(); break;
	case ISTORE_2:
		opt_ISTORE_2(); break;
	case ISTORE_3:
		opt_ISTORE_3(); break;
	case ICONST_0:
		opt_ICONST_0(); break;
	case ICONST_1:
		opt_ICONST_1(); break;
	case ICONST_2:
		opt_ICONST_2(); break;
	case ICONST_3:
		opt_ICONST_3(); break;
	case ICONST_4:
		opt_ICONST_4(); break;
	case ICONST_5:
		opt_ICONST_5(); break;
	case ILOAD_0:
		opt_ILOAD_0(); break;
	case ILOAD_1:
		opt_ILOAD_1(); break;
	case ILOAD_2:
		opt_ILOAD_2(); break;
	case ILOAD_3:
		opt_ILOAD_3(); break;
	case IINC:
		opt_IINC(); break;
	case BIPUSH:
		opt_BIPUSH(); break;
	case SIPUSH:
		opt_SIPUSH(); break;
	case LDC:
		opt_LDC(); break;
	case JMP:
		opt_JMP(); break;
	case JE:
		opt_JE(); break;
	case JNE:
		opt_JNE(); break;
	case JL:
		opt_JL(); break;
	case JG:
		opt_JG(); break;
	case EXIT:
		opt_EXIT(); break;
	default: {
			throw_error("unknown instruction : " + int_to_string(IR_Register));
		}
	}
}

void cycle_exit_clear_up() {
	/* do nothing */
}

void fetch_decode_execute_cycle() {

	initiating_cycle();
	while (!exit_flag) {
		#if __FOR_DEBUG
		show_stack_info();
		#endif
		fetch_instruction();
		decode_instruction();
		execute_instruction();
	}
	cycle_exit_clear_up();
}




} // namespace ministl

#endif // __MINI_VM_H
