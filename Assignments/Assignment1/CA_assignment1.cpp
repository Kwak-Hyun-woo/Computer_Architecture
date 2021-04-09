#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

// temp는 16진수 10진수 모두 될 수 있기 때문에 나누어 처리한다.
int string2int(string temp) {
	// 16진수(string) -> 10진수(int)
	int result;
	if (temp.size() > 2) {
		string check;
		check = temp.substr(0, 2);
		
		if (check == "0x") {         // 16진수 변환에 문제 발생
			result = stoul(temp, nullptr, 16);
		}
		// 10진수(string) -> 10진수(int)
		else {


			result = stoul(temp, nullptr, 10);
		}
	}
	else {
		result = stoul(temp, nullptr, 10);
	}

	return result;
}

string int2hex(unsigned int num){	// 32bit 이진수가 10진수로 저장되어 있는 것
	char buff[20];
	sprintf(buff, "0x%x", num);	
	string hex(buff);
	return hex;
}

// 각 형식에 따라 바이너리 구조 생성
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

// 각 줄마다 띄어쓰기를 기준으로 Tokenize하는 함수
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

// 맨 마지막 문자의 ":" 문자 여부로 label인지 아닌지 판별하는 함수 
bool is_label(string first_token) {
	if (first_token[first_token.size()- 1] == ':') {
		return true;
	}
	else {
		return false;
	}
}

// register를 정수형태로 변환하는 함수
unsigned int register_to_dec(string reg) {
	unsigned int result;
	result = string2int(reg.substr(1));		// '$'를 분리해주는 함수
	return result;
}

// num($num) 형태의 문자열을 offset과 register 값으로 나누어 vector에 저장하여 반환
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

// data_label를 구조체로 처리
struct label {
	unsigned int address;
	unsigned int word;
	string label_name;
};

// memory class
class Memory {
public:
	unsigned int PC = 0x00400000;			// text segmentation이 시작되는 주소값
	unsigned int Data_Address = 0x10000000;	// Data segmentation이 시작되는 주소값
	vector<label> labels;
	vector<unsigned int> words;

	void PC_update() { PC += 4;}
	void DA_update() { Data_Address += 4; }
	// word label 추가
	void add_label(label d_label) { labels.push_back(d_label); }
	// word 추가
	void add_word(unsigned int word) { words.push_back(word); }


	// data_words에서 해당 word_label의 주소 반환
	unsigned int label_address(string label){
		for(int i = 0; i < labels.size(); ++i) {
			if (labels[i].label_name == label) {
				return labels[i].address;
			}
		}
	}
};

// main 함수
int main(int argc, char* argv[]) {
	string assemblyfile = argv[1];		// assembly file 이름 저장
	ifstream readfile;					// file 객체 선언
	
	vector<string> line_tokens;			// 한줄씩 tokenize 

	string first_token;					// 각 라인의 첫번째 token을 의미하며 앞으로 어떤 동작을 할 지 결정
	string mode = "data";				// .data, .text에 따라 동작을 달리 하도록 결정시키는 문자열
	Memory memory;						

	readfile.open(assemblyfile);
	if (readfile.is_open()) {
		string line;

		// 미리 .data영역의 word와 label 주소, .text영역의 label 주소 입력
		while (getline(readfile, line)) {
			// line 비었을 때 넘길 수 있도록 처리
			if (line == "") { continue; }

			line_tokens = tokenize(line);
			first_token = line_tokens[0];
			if (first_token == ".word") {
				// word 처리(data 영역)
				memory.add_word(string2int(line_tokens[1]));
				memory.DA_update();
				mode = "data";
			}
			else if (mode == "data" && is_label(first_token)) {
				// label 처리(data 영역)
				label label;
				label.address = memory.Data_Address;
				label.label_name = first_token.substr(0, first_token.size() - 1);
				label.word = string2int(line_tokens[2]);
				memory.add_label(label);
				// 중복으로 memory의 vector<unsigned int> words에도 word를 넣어준다.
				memory.add_word(string2int(line_tokens[2]));
				memory.DA_update();
			}
			else if (first_token == ".text" || mode == "text") {
				mode = "text";
				if (first_token == ".text") { continue; }
				// label 처리(text 영역)
				if (is_label(first_token)) {
					label label;
					label.address = memory.PC;
					label.label_name = first_token.substr(0, first_token.size() - 1);
					memory.add_label(label);

					line_tokens.erase(line_tokens.begin());
				}
				// label 다음줄에 instruction이 있을 경우
				if (line_tokens.size() == 0) { continue; }

				//	la는 load address에 따라 instruction개수가 달라지므로 따로 처리
				if (first_token == "la") {
					unsigned int load_address = memory.label_address(line_tokens[2]);
					if ((load_address & 0b1111111111111111) == 0) {		// 하위 16bit 가 없음
						memory.PC_update();	// lui만 동작
					}
					else {												// 하위 16bit 가 있음
						memory.PC_update();	// lui 동작
						memory.PC_update(); // ori 동작
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
	mode = "data";	// mode를 data로 초기화


	vector<unsigned int> instructions;		// instruction 저장공간
	readfile.open(assemblyfile);
	if (readfile.is_open()) {
		string line;
		// 한 줄씩 tokenize하여 분석
		unsigned int cur_PC = 0x00400000;
		while (getline(readfile, line)) {
			// line 비었을 때 처리
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



				// label 처리(text 영역)
				if (is_label(first_token)) {
					line_tokens.erase(line_tokens.begin());
				}
				// label 다음줄에 instruction이 있을 경우
				if (line_tokens.size() == 0) { continue; }
				else { first_token = line_tokens[0]; }

				// instruction 처리(text 영역)
				// R format
				// op		rs		rt		rd		shamt	funct
				// 6bit		5bit	5bit	5bit	5bit	5bit
				// 줄에서 각각의 token들을 op, rs, rt, rd, shamt, funct에 할당
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
					shamt = string2int(line_tokens[3]);   // string to int 바꿔줘야함

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
					shamt = string2int(line_tokens[3]);   // string to int 바꿔줘야함
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
					// 의사 명령어
					// example) la $4 label
					unsigned int load_address = memory.label_address(line_tokens[2]);
					unsigned int upper_address = (load_address >> 16);
					unsigned int lower_address = (load_address & 0b1111111111111111);
					if (lower_address == 0b0000000000000000) {		// 하위 16bit 가 없음
						// lui 만 진행
						op_code = 0xf;
						rs = 0;
						rt = register_to_dec(line_tokens[1]);
						instruction = make_Iformat(op_code, rs, rt, upper_address);
						cur_PC += 4;
						instructions.push_back(instruction);
					
					}
					else {												// 하위 16bit 가 있음
						// lui, ori 진행
						op_code = 0xf;
						rs = 0;
						rt = register_to_dec(line_tokens[1]);
						instruction = make_Iformat(op_code, rs, rt, upper_address);
						cur_PC += 4;
						instructions.push_back(instruction);
						
						// ori 진행
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
	// assemblyfile의 .s 확장자 없애고 .o 새로운 확장자 붙이기
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