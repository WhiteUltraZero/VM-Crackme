#include <vector>
#include <iostream>
#include "vm.h"





VirtualMachine::VirtualMachine(std::vector<int64_t> code) {
		this->code = code;
		stack = std::vector<int64_t>(0x1100, 0);
}


void VirtualMachine::add_reg_imm(int64_t *reg, int64_t imm){
	*reg += imm;
}
void VirtualMachine::sub_reg_imm(int64_t* reg, int64_t imm) {
	*reg -= imm;
}
void VirtualMachine::add_reg_reg(int64_t *reg1, int64_t *reg2) {
	*reg1 += *reg2;
}
void VirtualMachine::sub_reg_reg(int64_t* reg1, int64_t* reg2) {
	*reg1 -= *reg2;
}
void VirtualMachine::add_reg_mem(int64_t* reg, uint8_t *mem) {
	*reg += *mem;
}

void VirtualMachine::sub_reg_mem(int64_t* reg, uint8_t* mem) {
	*reg -= *mem;
}
void VirtualMachine::add_mem_imm(uint8_t *mem, int64_t imm) {
	*mem += imm;
}
void VirtualMachine::sub_mem_imm(uint8_t* mem, int64_t imm) {
	*mem -= imm;
}
void VirtualMachine::add_mem_reg(uint8_t *mem, int64_t *reg) {
	*mem += *reg;
}
void VirtualMachine::sub_mem_reg(uint8_t* mem, int64_t* reg) {
	*mem -= *reg;
}
void VirtualMachine::add_mem_mem(uint8_t *mem1, uint8_t *mem2) {
	*mem1 += *mem2;
}
void VirtualMachine::sub_mem_mem(uint8_t* mem1, uint8_t* mem2) {
	*mem1 -= *mem2;
}
void VirtualMachine::push_imm(int64_t op1) {
	sp -= 1;
	stack[sp] = op1;
}
void VirtualMachine::push_reg(int64_t *reg){
	sp -= 1;
	stack[sp] = *reg;
}
void VirtualMachine::pop_reg(int64_t* reg) {
	*reg = stack[sp];
	sp += 1;
}

void VirtualMachine::cmp_reg_imm(int64_t* reg, int64_t imm) {
	if (*reg == imm) {
		this->zf = 1;
	}
	else {
		this->zf = 0;
	}
}

void VirtualMachine::cmp_reg_reg(int64_t* reg1, int64_t* reg2) {
	if (*reg1 == *reg2) {
		this->zf = 1;
	}
	else {
		this->zf = 0;
	}
}

void VirtualMachine::lte_reg_imm(int64_t* reg, int64_t imm) {
	if (*reg <= imm) {
		this->zf = 1;
	}
	else {
		this->zf = 0;
	}
}



void VirtualMachine::mov_reg_mem(int64_t* reg, uint8_t* mem) {
	*reg = *mem;
}

void VirtualMachine::mov_mem_reg(uint8_t* mem, int64_t* reg) {
	*mem = *reg;
}

void VirtualMachine::mov_reg_reg(int64_t* reg1, int64_t* reg2) {
	*reg1 = *reg2;
}

void VirtualMachine::mov_reg_reg_deref(int64_t* reg1, int64_t* reg2) {
	uint8_t* reg2_ptr = (uint8_t*)*reg2;
	uint8_t reg2_deref_val = *reg2_ptr;
	*reg1 = reg2_deref_val;
}


void VirtualMachine::mov_reg_imm(int64_t* reg, int64_t imm) {
	*reg = imm;
}




int64_t* VirtualMachine::register_decoder(uint64_t reg_enum) {
	switch (reg_enum) {
		case SP:
			return &this->sp;
		case RAX:
			return &this->rax;
		case RDX:
			return &this->rdx;
		case RBP:
			return &this->rbp;
		case RBX:
			return &this->rbx;
		case RCX:
			return &this->rcx;
		case RSI:
			return &this->rsi;
		case RDI:
			return &this->rdi;
		case CIP:
			return &this->cip;
		case ZF:
			return &this->zf;
		case R8:
			return &this->r8;
		default:
			//throw std::runtime_error("Unknown register enum");
			return &this->sp;
		};
	}

// Fetches 3 'bytes' although some instructions are only 2 bytes. They won't increment cip by 3 though
std::vector<int64_t> VirtualMachine::fetch() {
	std::vector<int64_t> instruction;
	instruction.push_back(code[cip]);
	instruction.push_back(code[cip + 1]);
	instruction.push_back(code[cip + 2]);
	return instruction;
}

void VirtualMachine::decode_and_dispatch(std::vector<int64_t> instruction) {
	uint64_t opcode = instruction[0];
	switch (opcode) {
	case ADD_REG_IMM_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		int64_t op2 = instruction[2];
		this->cip += 3;
		add_reg_imm(op1, op2);
		break;
	}

	case SUB_REG_IMM_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		int64_t op2 = instruction[2];
		this->cip += 3;
		sub_reg_imm(op1, op2);
		break;
	}

	case ADD_REG_REG_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		int64_t* op2 = register_decoder(instruction[2]);
		this->cip += 3;
		add_reg_reg(op1, op2);
		break;
	}

	case SUB_REG_REG_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		int64_t* op2 = register_decoder(instruction[2]);
		this->cip += 3;
		sub_reg_reg(op1, op2);
		break;
	}

	case ADD_REG_MEM_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		uint8_t* op2 = (uint8_t*)instruction[2];
		this->cip += 3;
		add_reg_mem(op1, op2);
		break;
	}

	case SUB_REG_MEM_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		uint8_t* op2 = (uint8_t*)instruction[2];
		this->cip += 3;
		sub_reg_mem(op1, op2);
		break;
	}

	case ADD_MEM_IMM_3: {
		uint8_t* op1 = (uint8_t*)instruction[1];
		int64_t op2 = instruction[2];
		this->cip += 3;
		add_mem_imm(op1, op2);
		break;
	}

	case SUB_MEM_IMM_3: {
		uint8_t* op1 = (uint8_t*)instruction[1];
		int64_t op2 = instruction[2];
		this->cip += 3;
		sub_mem_imm(op1, op2);
		break;
	}

	case ADD_MEM_REG_3: {
		uint8_t* op1 = (uint8_t*)instruction[1];
		int64_t* op2 = register_decoder(instruction[2]);
		this->cip += 3;
		add_mem_reg(op1, op2);
		break;
	}

	case SUB_MEM_REG_3: {
		uint8_t* op1 = (uint8_t*)instruction[1];
		int64_t* op2 = register_decoder(instruction[2]);
		this->cip += 3;
		sub_mem_reg(op1, op2);
		break;
	}

	case ADD_MEM_MEM_3: {
		uint8_t* op1 = (uint8_t*)instruction[1];
		uint8_t* op2 = (uint8_t*)instruction[2];
		this->cip += 3;
		add_mem_mem(op1, op2);
		break;
	}

	case SUB_MEM_MEM_3: {
		uint8_t* op1 = (uint8_t*)instruction[1];
		uint8_t* op2 = (uint8_t*)instruction[2];
		this->cip += 3;
		sub_mem_mem(op1, op2);
		break;
	}

	case POP_REG_2: {
		int64_t* op1 = register_decoder(instruction[1]);
		this->cip += 2;
		pop_reg(op1);
		break;
	}

	case PUSH_REG_2: {
		int64_t* op1 = register_decoder(instruction[1]);
		this->cip += 2;
		push_reg(op1);
		break;
	}

	case PUSH_IMM_2: {
		int64_t op1 = instruction[1];
		this->cip += 2;
		push_imm(op1);
		break;
	}

	case MOV_REG_IMM_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		int64_t op2 = instruction[2];
		this->cip += 3;
		mov_reg_imm(op1, op2);
		break;
	}

	case MOV_MEM_REG_3: {
		uint8_t* op1 = (uint8_t*)instruction[1];
		int64_t* op2 = register_decoder(instruction[2]);
		this->cip += 3;
		mov_mem_reg(op1, op2);
		break;
	}

	case MOV_REG_MEM_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		uint8_t * op2 = (uint8_t*)instruction[1];
		this->cip += 3;
		mov_reg_mem(op1, op2);
		break;
	}

	
	case MOV_REG_REG_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		int64_t* op2 = register_decoder(instruction[2]);
		this->cip += 3;
		mov_reg_reg(op1, op2);
		break;
	}

	case MOV_REG_REG_DEREF_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		int64_t* op2 = register_decoder(instruction[2]);
		this->cip += 3;
		mov_reg_reg_deref(op1, op2);
		break;
	}


	case CMP_REG_IMM_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		int64_t op2 = instruction[2];
		this->cip += 3;
		cmp_reg_imm(op1, op2);
		break;
	}

	case CMP_REG_REG_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		int64_t* op2 = register_decoder(instruction[2]);
		this->cip += 3;
		cmp_reg_reg(op1, op2);
		break;
	}

	case LTE_REG_IMM_3: {
		int64_t* op1 = register_decoder(instruction[1]);
		int64_t op2 = instruction[2];
		this->cip += 3;
		lte_reg_imm(op1, op2);
		break;
	}


	//Need to calculate jump offset, from the beginning of the JZ instruction, not from the end
	case JZ_2: {
		int64_t offset = instruction[1];

		if (this->zf == 1) {
			this->cip += offset;
		}
		else {
			this->cip += 2;
		}
		break;
	}

	case JNZ_2: {
		int64_t offset = instruction[1];

		if (this->zf == 0) {
			this->cip += offset;
		}
		else {
			this->cip += 2;
		}
		break;
	}

	case NULL_BYTE_1: {
		this->cip += 1;
		break;
	}

	case CONTROLLED_EXIT_CODE: {
		this->cip = CONTROLLED_EXIT_CODE;
		break;
	}

	default: {
		printf("Error occurred\n cip == %I64d", cip);
		this->cip = ERROR_CODE;
		break;
	}

	}

}

void VirtualMachine::run() {
	while ((cip != CONTROLLED_EXIT_CODE) && (cip != ERROR_CODE)) {
		decode_and_dispatch(VirtualMachine::fetch());
	}
}

