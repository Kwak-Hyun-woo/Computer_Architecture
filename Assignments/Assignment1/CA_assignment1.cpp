#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

// temp�� 16���� 10���� ��� �� �� �ֱ� ������ ������ ó���Ѵ�.
int string2int(string temp) {
	// 16����(string) -> 10����(int)
	int result;
	if (temp.size() > 2) {
		string check;
		check = temp.substr(0, 2);
		
		if (check == "0x") {         // 16���� ��ȯ�� ���� �߻�
			result = stoul(temp, nullptr, 16);
		}
		// 10����(string) -> 10����(int)
		else {


			result = stoul(temp, nullptr, 10);
		}
	}
	else {
		result = stoul(temp, nullptr, 10);
	}

	return result;
}

string int2hex(unsigned int num){	// 32bit �������� 10������ ����Ǿ� �ִ� ��
	char buff[20];
	sprintf(buff, "0x%x", num);	
	string hex(buff);
	return hex;
}

// �� ���Ŀ� ���� ���̳ʸ� ���� ����
unsigned int make_Rformat(unsigned int op, unsigned int rs, unsigned int rt, unsigned int  rd, unsigned int shamt, unsigned int  function){
	unsigned int instruction = 0;
	instruction |= op << 26;
	instruction |= rs << 21;
	instruction |= rt << 16;
	instruction |= rd << 11;
	instruction |= shamt << 6;
	instruction |= function;
	return instruction;
}
unsigned int make_Iformat(unsigned int op, unsigned int rs, unsigned int rt, unsigned int immediate) {
	unsigned int instruction = 0;
	instruction |= op << 26;
	instruction |= rs << 21;
	instruction |= rt << 16;
	instruction |= immediate;
	return instruction;
}
unsigned int make_Jformat(unsigned int op, unsigned int jump_target) {
	unsigned int instruction = 0;
	instruction |= op << 26;
	instruction |= jump_target;
	return instruction;
}

// �� �ٸ��� ���⸦ �������� Tokenize�ϴ� �Լ�
vector<string> tokenize(string line, const char delimiter = ' ') {
	vector<string> line_tokens;
	string token;
	stringstream ss(line);

	while (getline(ss, token, delimiter)) {
		if (token == ""){
			continue;
		}
		line_tokens.push_back(token);
	}
	return line_tokens;
}

// �� ������ ������ ":" ���� ���η� label���� �ƴ��� �Ǻ��ϴ� �Լ� 
bool is_label(string first_token) {
	if (first_token[first_token.size()- 1] == ':') {
		return true;
	}
	else {
		return false;
	}
}

// register�� �������·� ��ȯ�ϴ� �Լ�
unsigned int register_to_dec(string reg) {
	unsigned int result;
	result = string2int(reg.substr(1));		// '$'�� �и����ִ� �Լ�
	return result;
}

// num($num) ������ ���ڿ��� offset�� register ������ ������ vector�� �����Ͽ� ��ȯ
vector<unsigned int> register_with_offset_to_dec(string token) {
	vector<unsigned int> offset_register;
	string temp1 = "";
	string temp2 = "";
	int next;
	for (int i = 0; i < token.size(); ++i ) {
		if (token[i] == '(') {
			next = i;
			break;
		}
		temp1 += token[i];
	}
	for (int j = next + 2; j < token.size(); ++j) {
		if (token[j] == ')') {
			break;
		}
		temp2 += token[j];
	}
	unsigned int offset(string2int(temp1));
	unsigned int register_num(string2int(temp2));
	offset_register.push_back(offset);
	offset_register.push_back(register_num);
	return offset_register;
}

// data_label�� ����ü�� ó��
struct label {
	unsigned int address;
	unsigned int word;
	string label_name;
};

// memory class
class Memory {
public:
	unsigned int PC = 0x00400000;			// text segmentation�� ���۵Ǵ� �ּҰ�
	unsigned int Data_Address = 0x10000000;	// Data segmentation�� ���۵Ǵ� �ּҰ�
	vector<label> labels;
	vector<unsigned int> words;

	void PC_update() { PC += 4;}
	void DA_update() { Data_Address += 4; }
	// word label �߰�
	void add_label(label d_label) { labels.push_back(d_label); }
	// word �߰�
	void add_word(unsigned int word) { words.push_back(word); }


	// data_words���� �ش� word_label�� �ּ� ��ȯ
	unsigned int label_address(string label){
		for(int i = 0; i < labels.size(); ++i) {
			if (labels[i].label_name == label) {
				return labels[i].address;
			}
		}
	}
};

// main �Լ�
int main(int argc, char* argv[]) {
	string assemblyfile = argv[1];		// assembly file �̸� ����
	ifstream readfile;					// file ��ü ����
	
	vector<string> line_tokens;			// ���پ� tokenize 

	string first_token;					// �� ������ ù��° token�� �ǹ��ϸ� ������ � ������ �� �� ����
	string mode = "data";				// .data, .text�� ���� ������ �޸� �ϵ��� ������Ű�� ���ڿ�
	Memory memory;						

	readfile.open(assemblyfile);
	if (readfile.is_open()) {
		string line;

		// �̸� .data������ word�� label �ּ�, .text������ label �ּ� �Է�
		while (getline(readfile, line)) {
			// line ����� �� �ѱ� �� �ֵ��� ó��
			if (line == "") { continue; }

			line_tokens = tokenize(line);
			first_token = line_tokens[0];
			if (first_token == ".word") {
				// word ó��(data ����)
				memory.add_word(string2int(line_tokens[1]));
				memory.DA_update();
				mode = "data";
			}
			else if (mode == "data" && is_label(first_token)) {
				// label ó��(data ����)
				label label;
				label.address = memory.Data_Address;
				label.label_name = first_token.substr(0, first_token.size() - 1);
				label.word = string2int(line_tokens[2]);
				memory.add_label(label);
				// �ߺ����� memory�� vector<unsigned int> words���� word�� �־��ش�.
				memory.add_word(string2int(line_tokens[2]));
				memory.DA_update();
			}
			else if (first_token == ".text" || mode == "text") {
				mode = "text";
				if (first_token == ".text") { continue; }
				// label ó��(text ����)
				if (is_label(first_token)) {
					label label;
					label.address = memory.PC;
					label.label_name = first_token.substr(0, first_token.size() - 1);
					memory.add_label(label);

					line_tokens.erase(line_tokens.begin());
				}
				// label �����ٿ� instruction�� ���� ���
				if (line_tokens.size() == 0) { continue; }

				//	la�� load address�� ���� instruction������ �޶����Ƿ� ���� ó��
				if (first_token == "la") {
					unsigned int load_address = memory.label_address(line_tokens[2]);
					if ((load_address & 0b1111111111111111) == 0) {		// ���� 16bit �� ����
						memory.PC_update();	// lui�� ����
					}
					else {												// ���� 16bit �� ����
						memory.PC_update();	// lui ����
						memory.PC_update(); // ori ����
					}
				}
				else {
					memory.PC_update();
				}
			}
		}
		readfile.close();
	}
	else {
		cout << "fail opening file" << endl;
	}
	mode = "data";	// mode�� data�� �ʱ�ȭ


	vector<unsigned int> instructions;		// instruction �������
	readfile.open(assemblyfile);
	if (readfile.is_open()) {
		string line;
		// �� �پ� tokenize�Ͽ� �м�
		unsigned int cur_PC = 0x00400000;
		while (getline(readfile, line)) {
			// line ����� �� ó��
			if (line == "") { continue; }

			line_tokens = tokenize(line);
			first_token = line_tokens[0];
			if (mode == "text" || first_token == ".text") {
				mode = "text";
				if (first_token == ".text") { continue; }
				unsigned int op_code;
				unsigned int rs;
				unsigned int rt;
				unsigned int rd;

				unsigned int shamt;
				unsigned int funct;

				unsigned int offset;

				unsigned int jump_target;

				unsigned int instruction;
				int imm;



				// label ó��(text ����)
				if (is_label(first_token)) {
					line_tokens.erase(line_tokens.begin());
				}
				// label �����ٿ� instruction�� ���� ���
				if (line_tokens.size() == 0) { continue; }
				else { first_token = line_tokens[0]; }

				// instruction ó��(text ����)
				// R format
				// op		rs		rt		rd		shamt	funct
				// 6bit		5bit	5bit	5bit	5bit	5bit
				// �ٿ��� ������ token���� op, rs, rt, rd, shamt, funct�� �Ҵ�
				if (first_token == "addu") {
					// 0 - rs - rt - rd - 0 - 0x21
					// addu, rd, rs, rt
					op_code = 0;
					rs = register_to_dec(line_tokens[2]);
					rt = register_to_dec(line_tokens[3]);
					rd = register_to_dec(line_tokens[1]);
					shamt = 0;
					funct = 0x21;
					instruction = make_Rformat(op_code, rs, rt, rd, shamt, funct);
					instructions.push_back(instruction);
					cur_PC += 4;
				
				}
				else if (first_token == "and") {
					// 0 - rs - rt - rd - 0 - 0x24
					// and rd, rs, rt
					op_code = 0;
					rs = register_to_dec(line_tokens[2]);
					rt = register_to_dec(line_tokens[3]);
					rd = register_to_dec(line_tokens[1]);
					shamt = 0;
					funct = 0x24;
					instruction = make_Rformat(op_code, rs, rt, rd, shamt, funct);
					
					instructions.push_back(instruction);
					cur_PC += 4;
					
				}
				else if (first_token == "jr") {
					// 0 - rs - 0 - 0 - 0 - 8
					// jr rs
					op_code = 0;
					rs = register_to_dec(line_tokens[1]);
					rt = 0;
					rd = 0;
					shamt = 0;
					funct = 8;
					instruction = make_Rformat(op_code, rs, rt, rd, shamt, funct);
					instructions.push_back(instruction);
					cur_PC += 4;
					
				}
				else if (first_token == "nor") {
					// 0 - rs - rt - rd - 0 - 0x27
					// nor rd, rs, rt
					op_code = 0;
					rs = register_to_dec(line_tokens[2]);
					rt = register_to_dec(line_tokens[3]);
					rd = register_to_dec(line_tokens[1]);
					shamt = 0;
					funct = 0x27;
					instruction = make_Rformat(op_code, rs, rt, rd, shamt, funct);
					instructions.push_back(instruction);
					cur_PC += 4;
					
					
				}
				else if (first_token == "or") {
					// 0 - rs - rt - rd - 0 - 0x25
					// or rd, rs, rt
					op_code = 0;
					rs = register_to_dec(line_tokens[2]);
					rt = register_to_dec(line_tokens[3]);
					rd = register_to_dec(line_tokens[1]);
					shamt = 0;
					funct = 0x25;
					instruction = make_Rformat(op_code, rs, rt, rd, shamt, funct);
					instructions.push_back(instruction);
					cur_PC += 4;
					
				}
				else if (first_token == "sltu") {
					// 0 - rs - rt - rd - 0 - 0x2b
					// sltu rd, rs, rt
					op_code = 0;
					rs = register_to_dec(line_tokens[2]);
					rt = register_to_dec(line_tokens[3]);
					rd = register_to_dec(line_tokens[1]);
					shamt = 0;
					funct = 0x2b;
					instruction = make_Rformat(op_code, rs, rt, rd, shamt, funct);
					instructions.push_back(instruction);
					cur_PC += 4;
					
				}
				else if (first_token == "sll") {
					// 0 - 0 - rt - rd - shamt - 0
					// sll rd, rt, shamt
					op_code = 0;
					rs = 0;
					rt = register_to_dec(line_tokens[2]);
					rd = register_to_dec(line_tokens[1]);
					shamt = string2int(line_tokens[3]);   // string to int �ٲ������

					funct = 0;
					instruction = make_Rformat(op_code, rs, rt, rd, shamt, funct);
					instructions.push_back(instruction);
					cur_PC += 4;
				
				}
				else if (first_token == "srl") {
					// 0 - 0 - rt - rd - shamt - 2
					// srl rd, rt, shamt
					op_code = 0;
					rs = 0;
					rt = register_to_dec(line_tokens[2]);
					rd = register_to_dec(line_tokens[1]);
					shamt = string2int(line_tokens[3]);   // string to int �ٲ������
					funct = 2;

					instruction = make_Rformat(op_code, rs, rt, rd, shamt, funct);
					instructions.push_back(instruction);
					cur_PC += 4;
					
				}
				else if (first_token == "subu") {
					// 0 - rs - rt - rd - 0 - 0x23
					// subu rd, rs, rt
					op_code = 0;
					rs = register_to_dec(line_tokens[2]);
					rt = register_to_dec(line_tokens[3]);
					rd = register_to_dec(line_tokens[1]);
					shamt = 0;
					funct = 0x23;

					instruction = make_Rformat(op_code, rs, rt, rd, shamt, funct);
					instructions.push_back(instruction);
					cur_PC += 4;
				
				}
				// I Format
				// op		rs		rt		immediate
				// 6bit		5bit	5bit	16bit
				else if (first_token == "addiu") {
					// 9 - rs - rt - imm
					// addiu rt, rs, imm
					// example) addiu   $2, $0, 1024
					op_code = 9;
					rs = register_to_dec(line_tokens[2]);
					rt = register_to_dec(line_tokens[1]);
					imm = string2int(line_tokens[3]);
					if (imm < 0) {
						imm &= 0b00000000000000001111111111111111;
					}

					instruction = make_Iformat(op_code, rs, rt, imm);
					instructions.push_back(instruction);
					cur_PC += 4;
					
				}
				else if (first_token == "andi") {
					// Oxc - rs - rt - imm
					// andi rt, rs, imm
					// example) andi $14, $4, 100
					op_code = 0xc;
					rs = register_to_dec(line_tokens[2]);
					rt = register_to_dec(line_tokens[1]);
					imm = string2int(line_tokens[3]);
					if (imm < 0) {
						imm &= 0b00000000000000001111111111111111;
					}

					instruction = make_Iformat(op_code, rs, rt, imm);
					instructions.push_back(instruction);
					cur_PC += 4;
				
				}

				else if (first_token == "la") {
					// �ǻ� ��ɾ�
					// example) la $4 label
					unsigned int load_address = memory.label_address(line_tokens[2]);
					unsigned int upper_address = (load_address >> 16);
					unsigned int lower_address = (load_address & 0b1111111111111111);
					if (lower_address == 0b0000000000000000) {		// ���� 16bit �� ����
						// lui �� ����
						op_code = 0xf;
						rs = 0;
						rt = register_to_dec(line_tokens[1]);
						instruction = make_Iformat(op_code, rs, rt, upper_address);
						cur_PC += 4;
						instructions.push_back(instruction);
					
					}
					else {												// ���� 16bit �� ����
						// lui, ori ����
						op_code = 0xf;
						rs = 0;
						rt = register_to_dec(line_tokens[1]);
						instruction = make_Iformat(op_code, rs, rt, upper_address);
						cur_PC += 4;
						instructions.push_back(instruction);
						
						// ori ����
						op_code = 0xd;
						rs = register_to_dec(line_tokens[1]);
						rt = register_to_dec(line_tokens[1]);
						instruction = make_Iformat(op_code, rs, rt, lower_address);
						cur_PC += 4;
						instructions.push_back(instruction);
					
					}
				}
				else if (first_token == "beq") {
					// 4 - rs - rt - offset
					// beq rs, rt, label(offset)
					// example) beq $1, $0, sum_exit
					op_code = 4;
					rs = register_to_dec(line_tokens[1]);
					rt = register_to_dec(line_tokens[2]);
					offset = (memory.label_address(line_tokens[3]) - (cur_PC+4) ) / 4 ;
					instruction = make_Iformat(op_code, rs, rt, offset);
					instructions.push_back(instruction);
					cur_PC += 4;
				
				}
				else if (first_token == "bne") {
					// 5 - rs - rt - offset
					// bne - rs - rt - label(offset)
					// example) bne $1, $0, sum_exit
					op_code = 5;
					rs = register_to_dec(line_tokens[1]);
					rt = register_to_dec(line_tokens[2]);
					offset = (memory.label_address(line_tokens[3]) - (cur_PC+4)) / 4;
					instruction = make_Iformat(op_code, rs, rt, offset);
					instructions.push_back(instruction);
					cur_PC += 4;
				
				}
				else if (first_token == "lui") {
					// 0xf - 0 - rt - imm
					// lui rt, imm
					// example) lui $17, 100
					op_code = 0xf;
					rs = 0;
					rt = register_to_dec(line_tokens[1]);
					imm = string2int(line_tokens[2]);
					if (imm < 0) {
						imm &= 0b00000000000000001111111111111111;
					}

					instruction = make_Iformat(op_code, rs, rt, imm);
					instructions.push_back(instruction);
					cur_PC += 4;
				}
				else if (first_token == "lw") {
					// 0x23 - rs - rt - offset
					// lw rt, offset(rs)
					// example) lw $9, 0($8)
					op_code = 0x23;
					rs = register_with_offset_to_dec(line_tokens[2])[1];
					rt = register_to_dec(line_tokens[1]);
					instruction = make_Iformat(op_code, rs, rt, register_with_offset_to_dec(line_tokens[2])[0]);
					instructions.push_back(instruction);
					cur_PC += 4;
			
				}
				else if (first_token == "ori") {
					// 0xd - rs - rt - imm
					// ori rt, rs, imm
					// ori  $10, $2, 255
					op_code = 0xd;
					rs = register_to_dec(line_tokens[2]);
					rt = register_to_dec(line_tokens[1]);
					imm = string2int(line_tokens[3]);
					if (imm < 0) {
						imm &= 0b00000000000000001111111111111111;
					}

					instruction = make_Iformat(op_code, rs, rt, imm);
					instructions.push_back(instruction);
					cur_PC += 4;
				
				}
				else if (first_token == "sltiu") {
					// 0xb - rs - rt - imm
					// ori rt, rs, imm
					op_code = 0xb;
					rs = register_to_dec(line_tokens[2]);
					rt = register_to_dec(line_tokens[1]);
					imm = string2int(line_tokens[3]);
					if (imm < 0) {
						imm &= 0b00000000000000001111111111111111;
					}

					instruction = make_Iformat(op_code, rs, rt, imm);
					instructions.push_back(instruction);
					cur_PC += 4;
			
				}
				else if (first_token == "sw") {
					// 0x2b - rs - rt - offset
					// sw rt, offset(rs)
					op_code = 0x2b;
					rs = register_with_offset_to_dec(line_tokens[2])[1];
					rt = register_to_dec(line_tokens[1]);
					instruction = make_Iformat(op_code, rs, rt, register_with_offset_to_dec(line_tokens[2])[0]);
					instructions.push_back(instruction);
					cur_PC += 4;
	
				}
				else if (first_token == "lb") {
					// 0x20 - rs - rt - offset
					// lb  rt, offset(rs)
					op_code = 0x20;
					rs = register_with_offset_to_dec(line_tokens[2])[1];
					rt = register_to_dec(line_tokens[1]);
					instruction = make_Iformat(op_code, rs, rt, register_with_offset_to_dec(line_tokens[2])[0]);
					instructions.push_back(instruction);
					cur_PC += 4;
		
				}
				else if (first_token == "sb") {
					// 0x28 - rs - rt - offset
					// sb rt offset(rs) 
					// example) sb $2, 6($4)
					op_code = 0x28;
					rs = register_with_offset_to_dec(line_tokens[2])[1];
					rt = register_to_dec(line_tokens[1]);
					instruction = make_Iformat(op_code, rs, rt, register_with_offset_to_dec(line_tokens[2])[0]);
					instructions.push_back(instruction);
					cur_PC += 4;
			
				}
				// J Format
				// op		jump_target
				// 6bit		26bit
				else if (first_token == "j") {
					// 2 - target
					// j target
					op_code = 2;
					jump_target = memory.label_address(line_tokens[1]) / 4;
					instruction = make_Jformat(op_code, jump_target);
					instructions.push_back(instruction);
					cur_PC += 4;
				
				}
				else if (first_token == "jal") {
					// 3 - target
					// jal target
					op_code = 3;
					jump_target = memory.label_address(line_tokens[1]) / 4;
					instruction = make_Jformat(op_code, jump_target);
					instructions.push_back(instruction);
					cur_PC += 4;
			
				}
			}
		}
		readfile.close();
	}
	else {
		cout << "fail opening file" << endl;
	}
	

	ofstream writefile;
	// assemblyfile�� .s Ȯ���� ���ְ� .o ���ο� Ȯ���� ���̱�
	string outputfile = assemblyfile.substr(0, assemblyfile.size() - 2);
	outputfile += ".o";

	writefile.open(outputfile);
	// ---------- write file (output file) ----------
	// <text section size>
	writefile << int2hex(instructions.size() * sizeof(unsigned int)) << endl;
	// <data section size>
	writefile << int2hex(memory.words.size() * sizeof(unsigned int)) << endl;
	// <instructions>
	for (int i = 0; i < instructions.size(); ++i) {
		writefile << int2hex(instructions[i]) << endl;
	}
	// <values>
	for (int j = 0; j < memory.words.size(); ++j) {
		writefile << int2hex(memory.words[j]) << endl;
	}
	// ---------- write file (output file)----------
	writefile.close();
	
}