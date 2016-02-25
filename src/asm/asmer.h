// asmer.h

#ifndef __MINI_ASMER_H
#define __MINI_ASMER_H

// miniASM only allows '\s' '#' '.' ':' ',' 'a-z' 'A-Z' '-' '0-9' '_' '\"' .

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
	str, end_of_line
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
		throw "miniASM Syntax Error";
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

	void add_to_global_variable_map(const pair<string, size_t>& _pair) {
		global_variable_map.insert(_pair);
	}

	bool is_in_global_variable_map(const string& _varname) {
		return global_variable_map.find(_varname) != global_variable_map.end();
	}

	size_t get_global_variable_pos(const string& _varname) {
		return global_variable_map[_varname];
	}

	long long literal_to_number(const string& _literal) {

		stringstream _ss_tmp;
		_ss_tmp << _literal;

		if (_literal.find('.') == string::npos) {	// integral
			long long _integral;
			_ss_tmp >> _integral;
			return _integral;
		} else {	// float
			float _float;
			_ss_tmp >> _float;
			return static_cast<long long>(*((unsigned*)&_float));
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

	char hex_to_char(char a, char b) {
		char _c = 0;

		if (a >= '0' && a <= '9') { _c |= (a - '0'); }
		else if (a >= 'a' && a <= 'f') { _c |= (a - 'a' + 10); }
		else { throw_error("hex number is not valid."); }

		_c <<= 4;

		if (b >= '0' && b <= '9') { _c |= (b - '0'); }
		else if (b >= 'a' && b <= 'f') { _c |= (b - 'a' + 10); }
		else { throw_error("hex number is not valid."); }

		return _c;
	}

	string get_next_string() {	// for scanner
		size_t left = ++index_of_line;
		// i.e. s = " abc123, \t\"(%s)\"hey\n ";
		string _str;
		while (index_of_line < line_size && line[index_of_line] != '\"') {
			if (line[index_of_line] != '\\') {
				_str += line[index_of_line++];
			} else {
				++index_of_line;
				switch(line[index_of_line++]) {
				case 'a': _str += '\a'; break;
				case 'b': _str += '\b'; break;
				case 'f': _str += '\f'; break;
				case 'n': _str += '\n'; break;
				case 'r': _str += '\r'; break;
				case 't': _str += '\t'; break;
				case 'v': _str += '\v'; break;
				case '\\': _str += '\\'; break;
				case '?': _str += '\?'; break;
				case '\'': _str += '\''; break;
				case '\"': _str += '\"'; break;
				case '0': _str += '\0'; break;

				case 'x': 
					_str += hex_to_char(line[index_of_line], line[index_of_line + 1]);
					index_of_line += 2 ; break;
				default:
					_str += line[index_of_line - 2];
					_str += line[index_of_line - 1];
				#if 0
					throw_error(string("The character \'") 
						+ line[index_of_line]
						+ "\' is not a valid escape character");
				#endif
				}
			}
		}
		++index_of_line;	// discard the right '\"'
		return _str;
	}

	token next_token() {	// for scanner
		ignore_space();
		if (index_of_line >= line_size) { return token(end_of_line, "\n"); }

		char first_char = line[index_of_line++];
		if (first_char == '.') {
			return token(punctuation, ".");
		} else if (first_char == '\"') {
			--index_of_line;
			return token(str, get_next_string());
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
		throw_error("it should not be the asm directive");
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
			case ISTORE:
			case ILOAD:
			case FSTORE:
			case FLOAD:
		#if 0
			case ASTORE:
			case ALOAD:
		#endif
				add_bytecodes_to_ret(_num, 1); break;
			case SIPUSH:
			case ICONST:
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
			case PUTS:
			case IGLOBAL:
			case FGLOBAL:
				if (_tk_tmp.type == varname) {
					if (!is_in_global_variable_map(_tk_tmp.literal)) {
						throw_error("did not have the global variable \'"
							+ _tk_tmp.literal + "\'");
					} else {
						size_t _pos = get_global_variable_pos(_tk_tmp.literal);
						add_bytecodes_to_ret(static_cast<long long>(_pos), 4);
					}
				} else {
					throw_error("the operand of \'"
						+ _tk.literal
						+ "\' should be a variable name");
				} break;
			case LDC:
			case FCONST:
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


	void preprocess_asm_directive() {
		token _tk = next_token();
		if (_tk == token(varname, "limit")) {
			_tk = next_token();
			if (_tk == token(varname, "stack")) {
				_tk = next_token();
				if (_tk.type == number) {
					long long _num = literal_to_number(_tk.literal);
					asm_directive_area.push_back(DIRECTIVE_LIMIT);
					asm_directive_area.push_back(DIRECTIVE_STACK);
					size_t _occupied_size = 4;
					while (_occupied_size--) {
						asm_directive_area.push_back(
							static_cast<bytecode_type>(_num >> (_occupied_size * 8))
						);
					}
				} else {
					throw_error("expected a number of operand stack size");
				}
			} else if (_tk == token(varname, "locals")) {
				_tk = next_token();
				if (_tk.type == number) {
					long long _num = literal_to_number(_tk.literal);
					asm_directive_area.push_back(DIRECTIVE_LIMIT);
					asm_directive_area.push_back(DIRECTIVE_LOCALS);
					size_t _occupied_size = 4;
					while (_occupied_size--) {
						asm_directive_area.push_back(
							static_cast<bytecode_type>(_num >> (_occupied_size * 8))
						);
					}
				} else {
					throw_error("expected a number of locals param array size");
				}
			} else {
				throw_error("could not identify the operand \'"
					+ _tk.literal
					+ "\'. (maybe you mean \'stack\' or \'locals\'?)");
			}
		} else if (_tk == token(varname, "string")) {
			_tk = next_token();
			if (_tk.type == varname) {
				if (is_in_global_variable_map(_tk.literal)) {
					throw_error("redefined global variable \'"
						+ _tk.literal + "\'");
				}
				add_to_global_variable_map(
					pair<string, size_t>(_tk.literal, current_position())
				);

				_tk = next_token();
				if (_tk.type == str) {
					const char* _first = static_cast<const char*>(_tk.literal.c_str());
					const char* _last = _first + _tk.literal.size() + 1;
					ret.insert(ret.end(), _first, _last);
				} else {
					throw_error("expected a string literal");
				}
			} else {
				throw_error("expected a string name after the \'.string\'");
			}
		} else if (_tk == token(varname, "global")) {
			_tk = next_token();
			if (_tk.type == varname) {
				if (is_in_global_variable_map(_tk.literal)) {
					throw_error("redefined global variable \'"
						+ _tk.literal + "\'");
				} else {
					add_to_global_variable_map(
						// variable name and address
						pair<string, size_t>(_tk.literal, current_position())
					);
					_tk = next_token();
					if (_tk.type == number) {
						int _num = static_cast<int>(literal_to_number(_tk.literal));
						const char* _first = (const char*)&_num;
						const char* _last = _first + sizeof(_num);
						ret.insert(ret.end(), _first, _last);
					} else {
						throw_error("expected a number");
					}
				}
			} else {
				throw_error("expected a variable name after the \'.global\'");
			}
		} else {
			throw_error("could not identify the assembly directive \'"
				+ _tk.literal + "\'");
		}
	}

	void fill_asm_directive() {
		ret.insert(ret.end(), 
			asm_directive_area.begin(), asm_directive_area.end());
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
		asm_directive_area.reserve(16 * INSTRUCTION_BYTES);

		add_bytecodes_to_ret(0, 4);		// padding the room to fill the starting_PC
		add_bytecodes_to_ret(0, 4);		// padding the room to fill the size of static variable area

	}


	/* Scanner */
	void scan() {
		// fill the token stream
		// ignore chars after '#'
		string tmp;
		while (getline(ss, tmp)) {
            ++number_of_lines;

    // ************************************************* //
		#if __FOR_DEBUG
            cout << number_of_lines << " : ";
			if (tmp.length())
                cout << tmp << endl;
		#endif
    // ************************************************* //

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
			if (_tk == token(punctuation, ".")) {	// asm directive
				/* TODO(); deal with the asm directive */
				preprocess_asm_directive();
				continue;
			}

			while (_tk.type != end_of_line && _tk.type != unknown) {

		// ************************************************* //
			#if __FOR_DEBUG
				cout << "toekn(" << _tk.literal << ")" << endl;
			#endif
		// ************************************************* //

				token_stream.push_back(_tk);
				_tk = next_token();
			}
		}
	}


	/* Syntax Analyzer */
	void syntax_analyze() {

		long long asm_directive_starting_pos = static_cast<long long>(current_position());
		fill_asm_directive();
		long long instruction_starting_pos = static_cast<long long>(current_position());
		
		set_bytecodes_to_ret(instruction_starting_pos, 4, 0);
		set_bytecodes_to_ret(asm_directive_starting_pos, 4, 4);

	// **************************************** //
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
	map<string, size_t> global_variable_map;
	vector<pair<size_t, string> > label_fill_list;
	vector<char> asm_directive_area;
	stringstream ss;
	string line;
	size_t line_size;
	size_t number_of_lines;
	size_t index_of_line;	// for line
	size_t index_of_stream;	// for token_stream
};


} // namespace ministl

#endif // __MINI_ASMER_H
