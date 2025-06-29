#include <vector>
#include "vm.h"
#include <stdio.h>
#include "input.h"
#include <windows.h>
// Memory can only be accessed a byte at a time.



// All values must be passed in/ cast as int64_t. They get cast back to the original type within the VM.
// E.g if you want to pass in a memory address you cast it to a int64_t
// All memory operations are using masm syntax, i.e dereference is implied by mov.
std::vector<int64_t> bytecode = {

#define STRING_BASE RAX
#define COUNTER RCX
#define PTR_CUR_CHAR RBP
#define CUR_CHAR RDX
    MOV_REG_IMM_3, STRING_BASE, (int64_t)user_input,
	MOV_REG_IMM_3, COUNTER, 0,	

#define loop_start -15
// loop_start:
	MOV_REG_REG_3, PTR_CUR_CHAR, STRING_BASE,
	ADD_REG_REG_3, PTR_CUR_CHAR, COUNTER,
	ADD_REG_IMM_3, COUNTER, 1,
	MOV_REG_REG_DEREF_3, CUR_CHAR, PTR_CUR_CHAR,
	CMP_REG_IMM_3, CUR_CHAR, 0,
	JNZ_2, loop_start,//Offset to start of loop
			          // Null terminator found
#define COUNTER_2nd RDI

// Making sure input is at least 6 characters
#define BAD_LEN 40
	SUB_REG_IMM_3, COUNTER, 1,
	LTE_REG_IMM_3, COUNTER, 5,
	JZ_2, BAD_LEN,


#define SECRET_CHAR_VAR RSI
#define SECRET_CHAR_INC 4
#define SECRET_CHAR_INIT 66
	MOV_REG_IMM_3, COUNTER_2nd, 0,
	MOV_REG_IMM_3, SECRET_CHAR_VAR, SECRET_CHAR_INIT,


//Evaluating if correct string
#define CORRECT_CHAR_LOOP_START -26
#define CORRECT_STRING 26
// correct_char_loop_start:
	CMP_REG_REG_3, COUNTER_2nd, COUNTER,
	JZ_2, CORRECT_STRING,
	MOV_REG_REG_3, PTR_CUR_CHAR, STRING_BASE,
	ADD_REG_REG_3, PTR_CUR_CHAR, COUNTER_2nd,
	ADD_REG_IMM_3, COUNTER_2nd, 1,
	MOV_REG_REG_DEREF_3, CUR_CHAR, PTR_CUR_CHAR,
	SUB_REG_REG_3, CUR_CHAR, COUNTER_2nd,
	CMP_REG_REG_3, CUR_CHAR, SECRET_CHAR_VAR,
	ADD_REG_IMM_3, SECRET_CHAR_VAR, SECRET_CHAR_INC,
	JZ_2, CORRECT_CHAR_LOOP_START,
	CONTROLLED_EXIT_CODE,

// correct_string:
	MOV_REG_IMM_3, R8, 1,
// bad_len:
	CONTROLLED_EXIT_CODE, 0, 0, 0, 0,
 };
// Byte code must have some values after it otherwise it will cause an index error. Doesn't matter what the vals are, they won't be used



int main() {
	printf("Kevin Johnon's Crackme ---- Can you retrieve a valid password?\n");
	while (true) {
		get_user_input();
		//printf("%s", user_input);


		VirtualMachine vm = VirtualMachine(bytecode);
		vm.run();
		/*
		printf("\n");
		printf("Finished\n");
		printf("rcx ----> %I64d\n", vm.rcx);
		printf("rax ---->%I64d\n", vm.rax);
		printf("sp ---->%I64d\n", vm.sp);
		printf("rsi ---->%I64d\n", vm.rsi);
		printf("rbp ---->%I64d\n", vm.rbp);
		printf("rdi --->%I64d\n", vm.rdi);
		printf("rdx ---> %I64d\n", vm.rdx);
		printf("r8  --->%I64d\n", vm.r8);
		printf("zf  --->%I64d\n", vm.zf);
		*/
		if (vm.r8 == 1) {
			printf("Congratulations, correct string");
			Sleep(4000);
			break;
		}
		else {
			printf("Try again!\n");
		}
		clear_input_buffer();
	};
}

