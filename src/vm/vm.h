// vm.h

#ifndef __MINI_VM_H
#define __MINI_VM_H


#include <iostream>
#include <vector>
#include <cstdio>
#include <stdint.h>		// for int8_t and so on

#include "../mini_instructions.h"

/* Constand value */

#define LOCAL_PARAM_ARRAY_SIZE 8
#define OPERAND_STACK_SIZE 4
#define PROGRAM_STACK_SIZE 256

namespace ministl {

using std::vector;
using std::cerr;
using std::cout;
using std::endl;
using std::string;

typedef unsigned char bytecode_type;
typedef uint32_t sys_type;	// 32bit x86

class minivm {
public:
	minivm(size_t os_size = OPERAND_STACK_SIZE,
		size_t lpa_size = LOCAL_PARAM_ARRAY_SIZE,
		size_t ps_size = PROGRAM_STACK_SIZE) :
		Instruction_List(),
		PC_Register(0),
		IR_Register(NOP),
		Program_Stack(ps_size),
		Local_Param_Array(lpa_size),
		Operand_Stack(os_size),
		exit_flag(0) {}

public:
	/* API (could not be modified) */
	template <typename InputIterator>
	void load(InputIterator first, InputIterator last) {
		Instruction_List.assign(first, last);
	}

	void run() { fetch_decode_execute_cycle(); }



// ----------------------------------------------------------------------------
protected:

	/* About Error */
	void throw_error(const string& _err_str) {
		cerr << "[Error] : " << _err_str << endl;
	}

	void throw_error(const string& _err_str, bytecode_type _num) {
		cerr << _err_str << _num << endl;
	}


// ----------------------------------------------------------------------------
	/* Aux Functions */
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

	void set_variable(size_t _n, sys_type _val) {
		Local_Param_Array[_n] = _val;
	}

	sys_type get_variable(size_t _n) {
		return Local_Param_Array[_n];
	}

	inline bytecode_type get_next_Instruction() {
		return Instruction_List[PC_Register++];
	}

	long long get_next_n_instruction(size_t _n) {
		long long _ret = 0;
		for (unsigned i = 0; i < _n && i < sizeof(_ret); ++i) {
			_ret <<= 8;
			_ret += static_cast<long long>(get_next_Instruction());
		}
		return _ret;
	}

	// for opt_CALL
	void save_locals_array() {
		sys_type array_size = static_cast<sys_type>(Local_Param_Array.size());
		Program_Stack.insert(Program_Stack.end(),
				Local_Param_Array.begin(), Local_Param_Array.end());
		Program_Stack.push_back(array_size);
	}

	void save_operand_stack() {
		sys_type stack_size = static_cast<sys_type>(Operand_Stack.size());
		Program_Stack.insert(Program_Stack.end(),
				Operand_Stack.begin(), Operand_Stack.end());
		Program_Stack.push_back(stack_size);
	}

	void save_next_PC() {
		Program_Stack.push_back(PC_Register);
	}

	// for opt_RET
	void load_next_PC() {
		sys_type PC_n = Program_Stack.back();
		Program_Stack.pop_back();
		PC_Register = PC_n;
	}

	void load_operand_stack() {
		sys_type stack_size = Program_Stack.back();
		Program_Stack.pop_back();
		vector<sys_type>::iterator _it = Program_Stack.end();
		Operand_Stack.assign(_it - stack_size, _it);
	}

	void load_locals_array() {
		sys_type array_size = Program_Stack.back();
		Program_Stack.pop_back();
		vector<sys_type>::iterator _it = Program_Stack.end();
		Local_Param_Array.assign(_it - array_size, _it);
	}


// ----------------------------------------------------------------------------
	/* Instruction Opreations */
	void opt_NOP() {
		/* do nothing */
	}

	// Integral arithmetic
	void opt_IADD() {
		sys_type _val1 = pop_operand();
		sys_type _val2 = pop_operand();
		*((int *)&_val2) += *((int *)&_val1);
		push_operand(_val2);
	}

	void opt_ISUB() {
		sys_type _val1 = pop_operand();
		sys_type _val2 = pop_operand();
		*((int *)&_val2) -= *((int *)&_val1);
		push_operand(_val2);
	}

	void opt_IMUL() {
		sys_type _val1 = pop_operand();
		sys_type _val2 = pop_operand();
		*((int *)&_val2) *= *((int *)&_val1);
		push_operand(_val2);
	}

	void opt_IDIV() {
		sys_type _val1 = pop_operand();
		sys_type _val2 = pop_operand();
		*((int *)&_val2) /= *((int *)&_val1);
		push_operand(_val2);
	}

	void opt_MOD() {
		sys_type _val1 = pop_operand();
		sys_type _val2 = pop_operand();
		*((int *)&_val2) %= *((int *)&_val1);
		push_operand(_val2);
	}


	// Floating arithmetic
	
	void opt_FADD() {
		sys_type _val1 = pop_operand();
		sys_type _val2 = pop_operand();
		*((float *)&_val2) += *((float *)&_val1);
		push_operand(_val2);
	}

	void opt_FSUB() {
		sys_type _val1 = pop_operand();
		sys_type _val2 = pop_operand();
		*((float *)&_val2) -= *((float *)&_val1);
		push_operand(_val2);
	}

	void opt_FMUL() {
		sys_type _val1 = pop_operand();
		sys_type _val2 = pop_operand();
		*((float *)&_val2) *= *((float *)&_val1);
		push_operand(_val2);
	}

	void opt_FDIV() {
		sys_type _val1 = pop_operand();
		sys_type _val2 = pop_operand();
		*((float *)&_val2) /= *((float *)&_val1);
		push_operand(_val2);
	}

	// call
	void opt_PRINT() {
		sys_type _val = pop_operand();
		push_operand(_val);
		cout << _val;
	}

	void opt_IPRINT() {
		sys_type _val = pop_operand();
		printf("%d", *((int *)&_val));
	}

	void opt_FPRINT() {
		sys_type _val = pop_operand();
		printf("%f", *((float *)&_val));
	}

	void opt_EXIT() {
		exit_flag = true;
	}

	void opt_RET() {
		load_next_PC();
		// load_operand_stack();
		load_locals_array();
	}

	void opt_CALL() {

		sys_type PC_n = static_cast<sys_type>(get_next_n_instruction(2));

		save_locals_array();
		// save_operand_stack();
		save_next_PC();

		// update_PC
		PC_Register = PC_n;
	}

	// goto
	void opt_JMP() {
		sys_type PC_n = static_cast<sys_type>(get_next_n_instruction(2));
		PC_Register = PC_n;
	}

	void opt_JE() {
		sys_type PC_n = static_cast<sys_type>(get_next_n_instruction(2));
		sys_type _val = pop_operand();

		if (*((int*)&_val) == 0) { PC_Register = PC_n; }
	}

	void opt_JNE() {
		sys_type PC_n = static_cast<sys_type>(get_next_n_instruction(2));
		sys_type _val = pop_operand();

		if (*((int*)&_val) != 0) { PC_Register = PC_n; }
	}

	void opt_JL() {
		sys_type PC_n = static_cast<sys_type>(get_next_n_instruction(2));
		sys_type _val = pop_operand();

		if (*((int*)&_val) < 0) { PC_Register = PC_n; }
	}

	void opt_JLE() {
		sys_type PC_n = static_cast<sys_type>(get_next_n_instruction(2));
		sys_type _val = pop_operand();

		if (*((int*)&_val) <= 0) { PC_Register = PC_n; }
	}

	void opt_JG() {
		sys_type PC_n = static_cast<sys_type>(get_next_n_instruction(2));
		sys_type _val = pop_operand();

		if (*((int*)&_val) > 0) { PC_Register = PC_n; }
	}

	void opt_JGE() {
		sys_type PC_n = static_cast<sys_type>(get_next_n_instruction(2));
		sys_type _val = pop_operand();

		if (*((int*)&_val) >= 0) { PC_Register = PC_n; }
	}

	void opt_BIPUSH() {
		push_operand(static_cast<sys_type>(get_next_n_instruction(1)));
	}

	void opt_SIPUSH() {
		push_operand(static_cast<sys_type>(get_next_n_instruction(2)));
	}

	void opt_LDC() {
		push_operand(static_cast<sys_type>(get_next_n_instruction(4)));
	}

	void opt_IINC() {
		bytecode_type _val1 = get_next_Instruction();
		int8_t _val2 = static_cast<int8_t>(get_next_Instruction());

		size_t _n = static_cast<size_t>(_val1);
		sys_type _val = get_variable(_n);
		*((int*)&_val) += _val2;

		set_variable(_n, _val);
	}

	/* move function */
	void opt_ISTORE() {
		size_t _n = static_cast<size_t>(get_next_Instruction());
		sys_type _val = pop_operand();
		set_variable(_n, _val);
	}

	void opt_ILOAD() {
		size_t _n = static_cast<size_t>(get_next_Instruction());
		push_operand(get_variable(_n));
	}

	void opt_ICONST() {
		opt_BIPUSH();
	}

	void opt_FSTORE() {
		size_t _n = static_cast<size_t>(get_next_Instruction());
		sys_type _val = pop_operand();
		set_variable(_n, _val);
	}

	void opt_FLOAD() {
		size_t _n = static_cast<size_t>(get_next_Instruction());
		push_operand(get_variable(_n));
	}

	void opt_FCONST() {
		opt_LDC();
	}

	// Will be discarded
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





// ----------------------------------------------------------------------------
	/* Instruction Cycle */
	inline void initiating_cycle() {

		PC_Register = 0;
		IR_Register = NOP;

		Instruction_List.push_back(static_cast<bytecode_type>(EXIT));	// guard

		Program_Stack.clear();

		// guards
		Program_Stack.push_back(static_cast<sys_type>(0));	// locals array set zero
		// Program_Stack.push_back(static_cast<sys_type>(0));	// operand stack set zero
		// return to the last instruction, EXIT
		Program_Stack.push_back(static_cast<sys_type>(Instruction_List.size()));

		Local_Param_Array.clear();
		Operand_Stack.clear();

		exit_flag = false;
	}

	inline void fetch_instruction() {
		IR_Register = get_next_Instruction();
	}

	inline void decode_instruction() {
		/* do nothing */
	}

	inline void execute_instruction() {
		switch (IR_Register) {
		case NOP:
			opt_NOP(); break;

		// Integral
		case IADD:
			opt_IADD(); break;
		case ISUB:
			opt_ISUB(); break;
		case IMUL:
			opt_IMUL(); break;
		case IDIV:
			opt_IDIV(); break;
		case MOD:
			opt_MOD(); break;

		// Float
		case FADD:
			opt_FADD(); break;
		case FSUB:
			opt_FSUB(); break;
		case FMUL:
			opt_FMUL(); break;
		case FDIV:
			opt_FDIV(); break;

		// call
		case PRINT:
			opt_PRINT(); break;
		case IPRINT:
			opt_IPRINT(); break;
		case FPRINT:
			opt_FPRINT(); break;
		case EXIT:
			opt_EXIT(); break;
		case CALL:
			opt_CALL(); break;
		case RET:
			opt_RET(); break;
		case IINC:
			opt_IINC(); break;
		case BIPUSH:
			opt_BIPUSH(); break;
		case SIPUSH:
			opt_SIPUSH(); break;
		case LDC:
			opt_LDC(); break;

		// goto
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
		case JLE:
			opt_JLE(); break;
		case JGE:
			opt_JGE(); break;


		// move
		case ISTORE:
			opt_ISTORE(); break;
		case ILOAD:
			opt_ILOAD(); break;
		case ICONST:
			opt_ICONST(); break;
		case FSTORE:
			opt_FSTORE(); break;
		case FLOAD:
			opt_FLOAD(); break;
		case FCONST:
			opt_FCONST(); break;

		// Will be discarded
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


		default:
			throw_error("unknown instruction : ", IR_Register);
		}
	}

	inline void cycle_exit_clear_up() {
		/* do nothing */
	}

	void fetch_decode_execute_cycle() {

		initiating_cycle();

		while (!exit_flag) {

			fetch_instruction();
			decode_instruction();
			execute_instruction();
		}

		cycle_exit_clear_up();
	}


// ----------------------------------------------------------------------------

protected:
	/* Member Variables */
	vector<bytecode_type> Instruction_List;		// store program

	sys_type PC_Register;		// counter
	bytecode_type IR_Register;		// next instruction
	vector<sys_type> Program_Stack;		// program stack
	vector<sys_type> Local_Param_Array;		// store local variables
	vector<sys_type> Operand_Stack;		// store operands

	bool exit_flag;		// mark the running situation
};





} // namespace ministl

#endif // __MINI_VM_H
