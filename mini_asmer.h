/* 这是汇编器miniASMer */

// 目标是转汇编为机器码

#ifndef __MINI_ASMER_H
#define __MINI_ASMER_H



#define __FOR_DEBUG 0	// for debug


#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cctype>
#include <sstream>
#include <stack>

#include "mini_instruction_set.h"

#if __FOR_DEBUG
#include <cstdlib>	// for debug
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::istream;
using std::stringstream;
using std::getline;
using std::stack;

namespace ministl {

// 策略是 读入一个文件 每次读一行
// 以行为单位处理 空白行跳过
// 判断字符 手写自动机
// 若为小数点“.”开头则判定为汇编指令 暂时不作处理
// 若为字母开头 则判定为标号或者助记符
// 此时取整个单词 若单词后出现冒号 则为标号
// 若不为标号 则认定为助记符
// 此时从switch的case中查询是否存在此助记符
// 若不存在 则default报错
// 若存在 则进行处理
// 全程手写自动机 switch统治一切
// 注意行号！！！行号实质为字节码所在位置的索引号



typedef unsigned char bytecode_type;

// -------------------- 全局定义 ----------------------

static int line_number;		// 代码行号
static vector<bytecode_type> ret;
static int index;	// 这一行的第几个字符
static string line;
static int line_size;	// 此句的长度

static map<string, int> label_map;	// <name of label, the line number of label>
static vector<pair<int, string> > label_fill_list;



// --------------------- API ---------------------
// 传进一行 处理一行 返回vector打包的字节码

static void __parse_line(const string&);	// declaration

vector<bytecode_type> parse(istream& is) {
	string one_line;

	while (is) {
        getline(is, one_line);
        one_line = one_line.substr(0, one_line.find('#'));
		__parse_line(one_line);
		++line_number;
	}
#if 1
	for (unsigned i = 0; i < label_fill_list.size(); ++i) {
        int _num = label_map[label_fill_list[i].second];
        ret[label_fill_list[i].first] = static_cast<bytecode_type>(_num >> 8);
        ret[label_fill_list[i].first + 1] = static_cast<bytecode_type>(_num);
	}
#endif
	return ret;
}

// ---------------------------------------------------

/* 报错函数 */
static void __throw_error(const string& _err_str) {
	cerr << "[Error] : " << _err_str << endl;
#if __FOR_DEBUG
	system("pause");	// for debug
#endif
}



static long long string_to_int(const string& s) {
	long long _num;
	stringstream ss;
	ss << s;
	ss >> _num;
	return _num;
}

// ---------------------- parser -----------------------------

static void __parse();

static void __parse_line(const string& s) {
    // cout << line_number << " : " << s << endl;

	line = s;
	line_size = s.length();
	index = 0;

	__parse();
}



size_t instruction_pos() {
	return ret.size();
}

static bool is_in_label_map(const string& s) {
	return label_map.find(s) != label_map.end();
}

static void ignore_space() {
	// 跳过空白处 index指向非空格的第一个字符
	while (index < line_size && isspace(line[index])) { ++index; }
}

static void opt_asm_directive() {
	// 处理特定以“.”开头的汇编指令
	/* do nothing */
}

static string get_a_word() {
	// 扫描获取一个变量名
	ignore_space();
	int left = index, right = left;
	//cout << "index = " << index << endl;
	while (right < line_size &&
	 (isalpha(line[right]) || line[right] == '_' || isdigit(line[right]))) {
	 	//cout << "line[" << right << "] = " << line[right] << endl;
		++right;	// 符合变量名包含的字符则指向下一个字符 直到不符合
	}
	index = right;
	return line.substr(left, right - left);
}

static string get_a_token() {
	// 扫描获取一个变量名
	ignore_space();
	int left = index, right = left;
	//cout << "index = " << index << endl;
	while (right < line_size &&
	 !isspace(line[right]) && line[right] != ',') {
	 	//cout << "line[" << right << "] = " << line[right] << endl;
		++right;	// 符合变量名包含的字符则指向下一个字符 直到不符合
	}
	index = right;
	return line.substr(left, right - left);
}

static void push_num_of_n_bytes_into_ret(long long _num, unsigned _n) {
    stack<bytecode_type> st;
	for (unsigned i = 0; i < _n && i < sizeof(_num); ++i) {
		st.push(static_cast<bytecode_type>(_num >>= (8 * i)));
	}
	while (!st.empty()) {
        ret.push_back(st.top());
        st.pop();
	}
}

static void opt_label(const string& s) {
	label_map[s] = instruction_pos();
}

static void opt_instruction() {
	// 首先判断类型
	// cout << "rois is opt_instruction()" << endl;
	// 获取这一个token
	string _tmp = get_a_word();
	// cout << "get a word and word is " << _tmp << endl;

	// 取得对应的指令bytecode
	bytecode_type _instr = static_cast<bytecode_type>(instruction_to_bytecode(_tmp));

	if (_instr == static_cast<bytecode_type>(-1)) {	// label or wrong
		ignore_space();
		if (line[index] != ':') {
			__throw_error("Unknown instruction \"" + _tmp + "\"");
		} else {
			opt_label(_tmp);
		}
	} else {

		ret.push_back(_instr);

		if (occupied_bytes(_instr) > 1) {
			string _str = get_a_token();

			long long _num = 0;
			if (!isalpha(_str[0])) {
				_num = string_to_int(_str);
			}

			switch (_instr) {
			case BIPUSH:
				push_num_of_n_bytes_into_ret(_num, 1); break;
			case SIPUSH:
				push_num_of_n_bytes_into_ret(_num, 2); break;
			case IINC:
				push_num_of_n_bytes_into_ret(_num, 1);
				ignore_space();
				if (line[index] == ',') { ++index; }
				_num = string_to_int(get_a_token());
				push_num_of_n_bytes_into_ret(_num, 1);
				break;
			case JMP:
			case JE:
			case JG:
			case JGE:
			case JL:
			case JLE:
			case JNE:
				if (is_in_label_map(_str)) {
					push_num_of_n_bytes_into_ret(label_map[_str], 2);
				} else {
				    label_fill_list.push_back(pair<int, string>(instruction_pos(), _str));
				    push_num_of_n_bytes_into_ret(0, 2);
					// __throw_error("Label is undefined");
				} break;
			case LDC:
				push_num_of_n_bytes_into_ret(_num, 4); break;
			default:
				__throw_error("Unknown instruction \"" + _tmp + "\"");
			}
		}
	}

}

// 真正的处理
void __parse() {
	ignore_space();
	if (index >= line_size) return;

	char first_char = line[index];
	if (first_char == '.') {
		opt_asm_directive();
	} else if (isalpha(first_char)) {
		opt_instruction();
	} else {
		__throw_error(string("Syntax Error for first char \'") + first_char + "\'");
	}
}

} // namespace ministl

#endif // __MINI_ASMER_H
