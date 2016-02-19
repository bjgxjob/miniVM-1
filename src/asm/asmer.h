// asmer.h

#ifndef __MINI_ASMER_H
#define __MINI_ASMER_H

// miniASM only allows '\s' '#' '.' ':' ',' 'a-z' 'A-Z' '-' '0-9' '_' .

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include <sstream>
#include <cctype>

#include "../mini_instructions.h"

#define __FOR_DEBUG 0


namespace ministl {

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;


#define PADDING_OF_LABEL 0
#define INSTRUCTION_BYTES (sizeof(bytecode_type))
#define LABEL_BYTES 2


enum token_type {
	unknown, punctuation, label, mnemonic, integer, floating_number, varname, number,
	end_of_line
};

struct token {
	token_type type;
	string literal;

	token(token_type _tp, string _str) : type(_tp), literal(_str) {}
	bool operator==(const token& x) {
		return this->type == x.type && this->literal == x.literal;
	}
};



class miniasm {

	typedef unsigned char bytecode_type;

public:
	miniasm(char* _buf = NULL, size_t _len = 0) :
		buffer(_buf),
		size(_len),
		line_size(0),
		number_of_lines(0),
		index_of_line(0),
		index_of_stream(0) {}


public:
	/* API */
	void read(char* _buf, size_t _len) { buffer = _buf; size = _len; }

	vector<bytecode_type> write() { parse(); return ret; }


public:
	/* Aux Functions */
	void throw_error(const string& _err_str) {
		cerr << "[Error] : " << _err_str << endl;
	}

	void ignore_space() {	// for scanner
		while (index_of_line < line_size
			&& isspace(line[index_of_line])) {
			++index_of_line;
		}
	}

	size_t current_position() { return ret.size(); } // for syntax analyzer

	bool is_in_label_map(const string& _name) {
		return label_map.find(_name) != label_map.end();
	}

	void add_label(const string& _name) {
		if (is_in_label_map(_name)) {
			throw_error("redefined the label \'" + _name + "\'");
		}
		label_map[_name] = current_position();
	}

	size_t get_label_line(const string& _name) {
		if (is_in_label_map(_name)) {
			return label_map[_name];
		} else {
			throw_error("label \'" + _name + "\' is undefined");
			return -1;
		}
	}


	void add_bytecodes_to_ret(long long _data, size_t _occupied_size) {

		if (_occupied_size > sizeof(_data)) {
			throw_error("Could not add data with too large size into ret");
		}

		while (_occupied_size--) {
			ret.push_back(static_cast<bytecode_type>(_data >> (_occupied_size * 8)));
		}
	}


	void set_bytecodes_to_ret(long long _data, size_t _occupied_size, size_t _pos) {

		if (_occupied_size > sizeof(_data)) {
			throw_error("Could not add data with too large size into ret");
		}

		while (_occupied_size--) {
			ret[_pos++] = (static_cast<bytecode_type>(_data >> (_occupied_size * 8)));
		}
	}


	void add_to_label_fill_list(const string& _name, size_t _pos) {
		label_fill_list.push_back(pair<size_t, string>(_pos, _name));
	}

	void fill_label_in_ret() {

		for (unsigned i = 0; i < label_fill_list.size(); ++i) {

			size_t _pos = label_fill_list[i].first;
			size_t _label_line = get_label_line(label_fill_list[i].second);

			set_bytecodes_to_ret(static_cast<long long>(_label_line), LABEL_BYTES, _pos);
		}
	}

	long long literal_to_number(const string& _literal) {

		stringstream _ss_tmp;
		_ss_tmp << _literal;

		if (_literal.find('.') == string::npos) {	// integral
			long long _integral;
			_ss_tmp >> _integral;
			return _integral;
		} else {	// float
			double _float;
			_ss_tmp >> _float;
			long long *trans_point = (long long *)&_float;
			return *trans_point;
		}
	}


	string get_next_number() {	// for scanner
		size_t left = index_of_line;
		while (isdigit(line[index_of_line])
			|| line[index_of_line] == '.'
			|| line[index_of_line] == '-') {
			++index_of_line;
		}
		return line.substr(left, index_of_line - left);
	}

	string get_next_varname() {	// for scanner
		size_t left = index_of_line;
		while (isalpha(line[index_of_line])
			|| isdigit(line[index_of_line])
			|| line[index_of_line] == '_') {
			++index_of_line;
		}
		return line.substr(left, index_of_line - left);
	}

	token next_token() {	// for scanner
		ignore_space();
		if (index_of_line >= line_size) { return token(end_of_line, "\n"); }

		char first_char = line[index_of_line++];
		if (first_char == '.') {
			return token(punctuation, ".");
		} else if (first_char == ':') {
			return token(punctuation, ":");
		} else if (first_char == ',') {
			return token(punctuation, ",");
		} else if (isdigit(first_char) || first_char == '-') {
			--index_of_line;
			return token(number, get_next_number());
		} else if (isalpha(first_char) || first_char == '_') {
			--index_of_line;
			return token(varname, get_next_varname());
		} else {
			--index_of_line;
			throw_error("unknown token");
			return token(unknown, string("") + first_char);
		}
	}


	token get_next_token() {	// for syntax analyzer
		return token_stream[index_of_stream++];
	}

	token see_next_token() {	// for syntax analyzer
		return token_stream[index_of_stream];
	}

	void opt_asm_directive() {
		token _tk = get_next_token();

		if (_tk.literal == "limit") {
			if (see_next_token().type != varname) {
				throw_error("It should be a string after assembly directive");
			}

			token _tmp = get_next_token();
			if (_tmp.literal == "stack") {
				token _tk_tmp = get_next_token();
				long long _num = literal_to_number(_tk_tmp.literal);
				// TODO();
			} else if (_tmp.literal == "locals") {
				token _tk_tmp = get_next_token();
				long long _num = literal_to_number(_tk_tmp.literal);
				// TODO();
			} else {
				throw_error("could not identify the operand \'"
					+ _tmp.literal
					+ "\'. (maybe you mean \'stack\' or \'locals\'?)");
			}
		} else {
			throw_error("could not identify the assembly directive \'"
				+ _tk.literal + "\'");
		}
	}

	void opt_punctuation() {
		token _tk = get_next_token();
		if (_tk.literal == ".") {	// ams directive
			opt_asm_directive();
		} else {
			throw_error("The punctuation \'"
				+ _tk.literal
				+ "\' should not show up here");
		}
	}

	void opt_label(const token& _tk) {
		token _next_tk = see_next_token();
		if (_next_tk == token(punctuation, ":")) {	// label in head of line
			get_next_token();	// discard ':'
			add_label(_tk.literal);
		} else {	// label as the operand
			add_to_label_fill_list(_tk.literal, current_position());
			add_bytecodes_to_ret(static_cast<long long>(PADDING_OF_LABEL), LABEL_BYTES);
		}
	}

	void opt_mnemonic(const token& _tk) {

		bytecode_type _instr = static_cast<bytecode_type>(
			instruction_to_bytecode(_tk.literal)
		);

		if (_instr == static_cast<bytecode_type>(-1)) {
			throw_error("could not identify the mnemonic \'"
				+ _tk.literal + "\'");
		}

		add_bytecodes_to_ret(_instr, INSTRUCTION_BYTES);

		if (occupied_bytes(_instr) > 1) {	// has operands

			long long _num = 0;

			token _tk_tmp = get_next_token();

			if (_tk_tmp.type == number) {
				_num = literal_to_number(_tk_tmp.literal);
			}

			switch (_instr) {
			case BIPUSH:
				add_bytecodes_to_ret(_num, 1); break;
			case SIPUSH:
				add_bytecodes_to_ret(_num, 2); break;
			case IINC:
				add_bytecodes_to_ret(_num, 1);
				if (see_next_token() == token(punctuation, ",")) {
					get_next_token(); // discard ','
				}
				_tk_tmp = get_next_token();
				_num = literal_to_number(_tk_tmp.literal);
				add_bytecodes_to_ret(_num, 1);
				break;
			case CALL:
			case JMP:
			case JE:
			case JG:
			case JGE:
			case JL:
			case JLE:
			case JNE:
				if (_tk_tmp.type == varname) {
					opt_label(_tk_tmp);
				} else {
					throw_error("the operand of \'"
						+ _tk.literal
						+ "\' should be a label");
				} break;
			case LDC:
				add_bytecodes_to_ret(_num, 4); break;
			default:
				throw_error("Unknown instruction \"" + _tk.literal + "\"");
			}
		}
	}

	void opt_varname() {
		token _tk = get_next_token();

		if (see_next_token() == token(punctuation, ":")) {	// label
			opt_label(_tk);
		} else {
			opt_mnemonic(_tk);
		}
	}

	void opt_number() {
		token _tk = get_next_token();
		throw_error("Number \'"
			+ _tk.literal
			+ "\' should not appear separately");
	}


	/* Initializer */
	void init() {

		ss.write(buffer, size);

		number_of_lines = 0;
		index_of_line = 0;
		index_of_stream = 0;
		ret.clear();
		token_stream.clear();
		label_map.clear();
		label_fill_list.clear();
	}


	/* Scanner */
	void scan() {
		// fill the token stream
		// ignore chars after '#'
		string tmp;
		while (getline(ss, tmp)) {
            ++number_of_lines;
		#if __FOR_DEBUG
            cout << number_of_lines << " : ";
			if (tmp.length())
                cout << tmp << endl;
		#endif

			if (tmp.length() == 0) { continue; }

			size_t _pos = tmp.find('#');
			if (_pos != string::npos) {
				line = tmp.substr(0, _pos);
			} else {
				line = tmp;
			}

            index_of_line = 0;
			line_size = line.length();

			if (line_size == 0) { continue; }

			token _tk = next_token();

			while (_tk.type != end_of_line && _tk.type != unknown) {

			#if __FOR_DEBUG
				cout << "toekn(" << _tk.literal << ")" << endl;
			#endif

				token_stream.push_back(_tk);
				_tk = next_token();
			}
		}
	}


	/* Syntax Analyzer */
	void syntax_analyze() {

		index_of_stream = 0;	// init

		size_t _Size = token_stream.size();
		token_stream.push_back(token(end_of_line, "\n"));	// guard

		while (index_of_stream < _Size) {

			token _tk = see_next_token();

			switch (_tk.type) {

			case punctuation:
				opt_punctuation(); break;
			case varname:
				opt_varname(); break;
			case number:
				opt_number(); break;
			default:
				throw_error("could not identify the token \'"
					+ _tk.literal + "\'");
			}
		}
	}


	/* Code Generator */
	void code_generate() {
		fill_label_in_ret();
	}



	/* Parser */
	void parse() {
		init();
		scan();
		syntax_analyze();
		code_generate();
	}


protected:
	/* Member Variables */
	const char *buffer;
	size_t size;
	vector<bytecode_type> ret;
	vector<token> token_stream;
	map<string, size_t> label_map;
	vector<pair<size_t, string> > label_fill_list;
	stringstream ss;
	string line;
	size_t line_size;
	size_t number_of_lines;
	size_t index_of_line;	// for line
	size_t index_of_stream;	// for token_stream
};


} // namespace ministl

#endif // __MINI_ASMER_H
