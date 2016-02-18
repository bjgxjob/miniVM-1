# 大量重复工作

# 输入指令集 生成特定字符串

import re

def deal_case_return(word):
	return "case " + word + ": return \"" + word + "\";"

def deal1(word):
	return 'm.insert(pair<string, int>("' + word + '", ' + word+ '));'

def deal(word):
	return 'pair<string, int>("' + word + '", ' + word+ '),'

def predeal(s):
	_tmp = re.findall(r'[a-zA-Z\_][0-9a-zA-Z\_]*', s)
	lis = list(map(deal, _tmp))
	for i in lis:
		print(i)

def main():
	#ss = input("enter the instruction set, separated by ', ':")
	ss = '''
	NOP = 0,

	EXIT = 128,

	IADD = 129, 
	ISUB = 130, 
	IMUL = 131, 
	IDIV = 132,

	BIPUSH = 136, 
	SIPUSH = 137,
	LDC = 138,
	IINC = 139, 
	PRINT = 140, 
	JMP = 141, 

	ISTORE_0 = 142, 
	ISTORE_1 = 143, 
	ISTORE_2 = 144, 
	ISTORE_3 = 145,

	ILOAD_0 = 146, 
	ILOAD_1 = 147, 
	ILOAD_2 = 148, 
	ILOAD_3 = 149,

	ICONST_0 = 150, 
	ICONST_1 = 151, 
	ICONST_2 = 152, 
	ICONST_3 = 153, 
	ICONST_4 = 154, 
	ICONST_5 = 155,

	JE = 156, 
	JG = 157,
	JGE = 158,
	JL = 159,
	JLE = 160,
	JNE = 161,

	READLINE = 180, 
	RET = 181, 
	CALL = 182, 
	'''
	predeal(ss)

if __name__ == '__main__':
	main()