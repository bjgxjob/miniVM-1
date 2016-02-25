// instruction list

#ifndef __MINI_INSTRUCTIONS_H
#define __MINI_INSTRUCTIONS_H

#include <string>
#include <map>
#include <vector>
#include <cctype>
using std::string;
using std::map;
using std::vector;
using std::pair;

/*
	Note:
		If you want to add some instructions into the instructions
			set, you should modify three parts of this file.
		The first is enum, which defines the mnemonics of instructions.
		The second is the array sz[], which store pairs of mnemonics and its string.
		The third is the function bytecode_to_instruction().

		By the way, you should add the corresponding opt_INS() 
			functions and the execute_cycle.
*/



namespace ministl {

// instruction bytecode
enum {
	// pop
	// pop2
	// getaddr
	// 

/* 1 byte occupied */
	NOP = 0,

	EXIT = 128,
	RET,
	INFO,

	PRINT,
	IPRINT,
	FPRINT,
	CPRINT,

	IADD,
	ISUB,
	IMUL,
	IDIV,
	MOD,

	FADD,
	FSUB,
	FMUL,
	FDIV,

	ISTORE_0,
	ISTORE_1,
	ISTORE_2,
	ISTORE_3,

	ILOAD_0,
	ILOAD_1,
	ILOAD_2,
	ILOAD_3,

	ICONST_0,
	ICONST_1,
	ICONST_2,
	ICONST_3,
	ICONST_4,
	ICONST_5,

	DUP,
	DUP2,

/* 2 byte occupied */
	BIPUSH,

	ISTORE,
	ILOAD,
	FSTORE,
	FLOAD,
	ASTORE,
	ALOAD,

/* 3 byte occupied */
	SIPUSH,
	IINC,

	ICONST,

	JMP,
	JE,
	JG,
	JGE,
	JL,
	JLE,
	JNE,

	CALL,

/* 5 byte occupied */
	LDC,
	FCONST,
	PUTS,
	IGLOBAL,
	FGLOBAL,

};



static pair<const string, int> sz[] = {
	pair<const string, int>("NOP", NOP),
	pair<const string, int>("EXIT", EXIT),
	pair<const string, int>("RET", RET),
	pair<const string, int>("INFO", INFO),

	pair<const string, int>("IADD", IADD),
	pair<const string, int>("ISUB", ISUB),
	pair<const string, int>("IMUL", IMUL),
	pair<const string, int>("IDIV", IDIV),
	pair<const string, int>("BIPUSH", BIPUSH),
	pair<const string, int>("SIPUSH", SIPUSH),
	pair<const string, int>("LDC", LDC),
	pair<const string, int>("IINC", IINC),
	pair<const string, int>("PRINT", PRINT),
	pair<const string, int>("JMP", JMP),
	pair<const string, int>("ISTORE", ISTORE),
	pair<const string, int>("ILOAD", ILOAD),
	pair<const string, int>("ICONST", ICONST),
	pair<const string, int>("ISTORE_0", ISTORE_0),
	pair<const string, int>("ISTORE_1", ISTORE_1),
	pair<const string, int>("ISTORE_2", ISTORE_2),
	pair<const string, int>("ISTORE_3", ISTORE_3),
	pair<const string, int>("ILOAD_0", ILOAD_0),
	pair<const string, int>("ILOAD_1", ILOAD_1),
	pair<const string, int>("ILOAD_2", ILOAD_2),
	pair<const string, int>("ILOAD_3", ILOAD_3),
	pair<const string, int>("ICONST_0", ICONST_0),
	pair<const string, int>("ICONST_1", ICONST_1),
	pair<const string, int>("ICONST_2", ICONST_2),
	pair<const string, int>("ICONST_3", ICONST_3),
	pair<const string, int>("ICONST_4", ICONST_4),
	pair<const string, int>("ICONST_5", ICONST_5),
	pair<const string, int>("DUP", DUP),
	pair<const string, int>("DUP2", DUP2),
	pair<const string, int>("JE", JE),
	pair<const string, int>("JG", JG),
	pair<const string, int>("JGE", JGE),
	pair<const string, int>("JL", JL),
	pair<const string, int>("JLE", JLE),
	pair<const string, int>("JNE", JNE),
	pair<const string, int>("CALL", CALL),

	pair<const string, int>("IPRINT", IPRINT),
	pair<const string, int>("FPRINT", FPRINT),
	pair<const string, int>("CPRINT", CPRINT),
	pair<const string, int>("MOD", MOD),
	pair<const string, int>("FADD", FADD),
	pair<const string, int>("FSUB", FSUB),
	pair<const string, int>("FMUL", FMUL),
	pair<const string, int>("FDIV", FDIV),
	pair<const string, int>("FSTORE", FSTORE),
	pair<const string, int>("FLOAD", FLOAD),
	pair<const string, int>("FCONST", FCONST),

	pair<const string, int>("PUTS", PUTS),
	pair<const string, int>("IGLOBAL", IGLOBAL),
	pair<const string, int>("FGLOBAL", FGLOBAL),

	pair<const string, int>("ASTORE", ASTORE),
	pair<const string, int>("ALOAD", ALOAD),
};

static map<string, int> map_for_instr_and_bytecode(
	sz,	sz + (sizeof(sz) / sizeof(sz[0]))
);

#if 1
string bytecode_to_instruction(int _opcode) {
	switch (_opcode) {
	case NOP : return "NOP";
	case EXIT : return "EXIT";
	case RET : return "RET";
	case INFO : return "INFO";

	case IADD : return "IADD";
	case ISUB : return "ISUB";
	case IMUL : return "IMUL";
	case IDIV : return "IDIV";

	case BIPUSH : return "BIPUSH";
	case SIPUSH : return "SIPUSH";
	case LDC : return "LDC";
	case IINC : return "IINC";
	case PRINT : return "PRINT";
	case JMP : return "JMP";

	case ISTORE : return "ISTORE";
	case ILOAD : return "ILOAD";
	case ICONST : return "ICONST";

	case ISTORE_0 : return "ISTORE_0";
	case ISTORE_1 : return "ISTORE_1";
	case ISTORE_2 : return "ISTORE_2";
	case ISTORE_3 : return "ISTORE_3";

	case ILOAD_0 : return "ILOAD_0";
	case ILOAD_1 : return "ILOAD_1";
	case ILOAD_2 : return "ILOAD_2";
	case ILOAD_3 : return "ILOAD_3";

	case ICONST_0 : return "ICONST_0";
	case ICONST_1 : return "ICONST_1";
	case ICONST_2 : return "ICONST_2";
	case ICONST_3 : return "ICONST_3";
	case ICONST_4 : return "ICONST_4";
	case ICONST_5 : return "ICONST_5";

	case DUP : return "DUP";
	case DUP2 : return "DUP2";

	case JE : return "JE";
	case JG : return "JG";
	case JGE : return "JGE";
	case JL : return "JL";
	case JLE : return "JLE";
	case JNE : return "JNE";
	case CALL : return "CALL";

	case IPRINT : return "IPRINT";
	case FPRINT : return "FPRINT";
	case CPRINT : return "CPRINT";
	case MOD : return "MOD";
	case FADD : return "FADD";
	case FSUB : return "FSUB";
	case FMUL : return "FMUL";
	case FDIV : return "FDIV";
	case FSTORE : return "FSTORE";
	case FLOAD : return "FLOAD";
	case FCONST : return "FCONST";

	case PUTS : return "PUTS";
	case IGLOBAL : return "IGLOBAL";
	case FGLOBAL : return "FGLOBAL";

	case ASTORE : return "ASTORE";
	case ALOAD : return "ALOAD";

	default: return "Unknown";
	}
}
#endif

static string word_to_upper(const string& s) {
	string _tmp(s);
	for (unsigned i = 0; i < s.length(); ++i) {
		if (isalpha(s[i])) _tmp[i] = toupper(s[i]);
	}
	return _tmp;
}

int instruction_to_bytecode(const string& _instr) {
	string _tmp = word_to_upper(_instr);
	if (map_for_instr_and_bytecode.find(_tmp) == map_for_instr_and_bytecode.end()) {
		return -1;
	}
	return map_for_instr_and_bytecode[_tmp];
}

static int top_of_n_bytes_occupied(int _n) {
	switch (_n) {
	case 1: return EXIT;
	case 2: return BIPUSH;
	case 3: return SIPUSH;
	case 5: return LDC;
	default: throw;
	}
}

int occupied_bytes(int _n) { 
	if (_n < top_of_n_bytes_occupied(2)) {
		return 1;
	} else if (_n < top_of_n_bytes_occupied(3)) {
		return 2;
	} else if (_n < top_of_n_bytes_occupied(5)) {
		return 3;
	} else {
		return 5;
	}
}


enum {
	DIRECTIVE_LIMIT = 1,
	DIRECTIVE_STACK,
	DIRECTIVE_LOCALS,
	DIRECTIVE_GLOBAL,
	DIRECTIVE_FLOAT,
	DIRECTIVE_INTEGER,
	// DIRECTIVE_ADDRESS,
	DIRECTIVE_STRING,
};


} // namespace ministl

#endif // __MINI_INSTRUCTIONS_H