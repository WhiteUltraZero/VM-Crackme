#include <vector>
#include <stdio.h>

char user_input[256] = { 0 };

void get_user_input() {
	printf("Enter the password: \n");
	scanf_s("%255s", user_input, 256);
}

void clear_input_buffer() {
	for (int i = 0; i < 256; i++) {
		user_input[i] = 0;
	}
};
