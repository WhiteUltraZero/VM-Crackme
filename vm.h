#pragma once
#include <vector>
#include <cstdint>




enum Opcode {
	ADD_REG_IMM_3 = 0x10001,
	SUB_REG_IMM_3 = 0x10002,
	ADD_REG_REG_3 = 0x10005,
	SUB_REG_REG_3 = 0x10004,
	ADD_REG_MEM_3 = 0x10006,
	SUB_REG_MEM_3 = 0x10010,
	ADD_MEM_IMM_3 = 0x10007,
	SUB_MEM_IMM_3 = 0x10011,
	ADD_MEM_REG_3 = 0x10008,
	ADD_MEM_MEM_3 = 0x10009,
	SUB_MEM_REG_3 = 0x10012,
	SUB_MEM_MEM_3 = 0x10013,
	PUSH_REG_2 = 0x10014,
	PUSH_IMM_2 = 0x10015,
	POP_REG_2 = 0x10016,
	CMP_REG_IMM_3 = 0x10017,
	CMP_REG_REG_3 = 0x10050,
	MOV_REG_MEM_3 = 0x10031,
	MOV_REG_REG_3 = 0x10032,
	JZ_2 = 0x10018,
	JNZ_2 = 0x10019,
	MOV_MEM_REG_3 = 0x10020,
	MOV_REG_IMM_3 = 0x10022,
	MOV_REG_REG_DEREF_3 = 0x10023,
	NULL_BYTE_1 = 0x9009,
	ERROR_CODE = 0x7FFFFFFF,
	CONTROLLED_EXIT_CODE = 0x7FFFFFFE,
	LTE_REG_IMM_3 = 0x12000
};

enum Register {
	SP = 0x5FFFFFF1,
	RAX = 0x5FFFFFF2,
	RDX = 0x5FFFFFF3,
	RBP = 0x5FFFFFF4,
	RBX = 0x5FFFFFF5,
	RCX = 0x5FFFFFF6,
	RSI = 0x5FFFFFF7,
	RDI = 0x5FFFFFF8,
	R8  = 0x5FFFFFFC,
	CIP = 0x5FFFFFF9,
	ZF  = 0x5FFFFFFA
};

class VirtualMachine {
public:
	int64_t cip = 0;

	int64_t sp  = 0x1000;
	int64_t rax = 0;
	int64_t rdx = 0;
	int64_t rbp = 0;
	int64_t rbx = 0;
	int64_t rcx = 0;
	int64_t rsi = 0;
	int64_t rdi = 0;
	int64_t r8 = 0;
	int64_t  zf = 0;

	std::vector<int64_t> code;
	std::vector<int64_t> stack;

	VirtualMachine(std::vector<int64_t> code);

	void add_reg_imm(int64_t* reg, int64_t imm);
	void sub_reg_imm(int64_t* reg, int64_t imm);
	void add_reg_reg(int64_t* reg1, int64_t* reg2);
	void sub_reg_reg(int64_t* reg1, int64_t* reg2);
	void add_reg_mem(int64_t* reg, uint8_t* mem);
	void sub_reg_mem(int64_t* reg, uint8_t* mem);
	void add_mem_imm(uint8_t* mem, int64_t imm);
	void sub_mem_imm(uint8_t* mem, int64_t imm);
	void add_mem_reg(uint8_t* mem, int64_t* reg);
	void sub_mem_reg(uint8_t* mem, int64_t* reg);
	void add_mem_mem(uint8_t* mem1, uint8_t* mem2);
	void sub_mem_mem(uint8_t* mem1, uint8_t* mem2);
	void push_imm(int64_t op1);
	void push_reg(int64_t* reg);
	void pop_reg(int64_t* reg);
	void cmp_reg_imm(int64_t* reg, int64_t imm);
	void cmp_reg_reg(int64_t* reg1, int64_t* reg2);
	void mov_mem_reg(uint8_t* mem, int64_t* reg);
	void mov_reg_imm(int64_t* reg, int64_t imm);
	void mov_reg_mem(int64_t* reg, uint8_t* mem);
	void mov_reg_reg(int64_t* reg1, int64_t* reg2);
	void mov_reg_reg_deref(int64_t* reg1, int64_t* reg2);
	void lte_reg_imm(int64_t* reg, int64_t imm); //lte == less than or equal to

	int64_t* register_decoder(uint64_t reg_enum);

	std::vector<int64_t> fetch();

	void decode_and_dispatch(std::vector<int64_t> instruction);

	void run();
};

// Memory can only be accessed a byte at a time.



