#include "printf.h"
#include "uart.h"

#define MAX_PRINT_SIZE 256

void rst_buff(char *ar) {
	int n = 0;
	while(ar[n] != '\0') {
		ar[n] = '\0';
		n++;
	}
}

void specifiers(int *index, char *str, char *arr) {
	char container[] = {'c', 'd', 's', 'x', '%', 'f'};

	while(*str != '\n' && *str != ' ') {
		str++;
		*arr = *str;
		arr++;
		*index = index + 1;

		for(int i = 0; i < 6; i++){
			if(*str == container[i]) {
				return;
			}
		}
	}
}

int power(int base_num, int exponent) {
	int pow = base_num;
	for(int i = 1; i < exponent; i++) {
		pow *= base_num;
	}
	return pow;
}

void printf(char *string,...) {

	va_list ap;
	va_start(ap, string);

	char buffer[MAX_PRINT_SIZE];
	int buffer_index = 0;
	char buff_container[MAX_PRINT_SIZE];
	char buff_specifier[MAX_PRINT_SIZE];
	char temp_buffer[MAX_PRINT_SIZE];

	rst_buff(buffer);
	rst_buff(buff_specifier);

	while(1) {
		if (*string == 0)
			break;

		if (*string == '%') {
			if(*(string + 1) == '%'){
				string++;
				while(*string != ' ' && *string != '\0') {
					buffer[buffer_index] = *string;
					buffer_index++;
					string++;
				}
				continue;
			}

			int specifier = 0;
			specifiers(&specifier, string, buff_specifier);
			string += (specifier + 1);
			
			int ind_holder = 0;
			int end = -1; 
			int hold_number = 0;
			int n = 0;
			int negative_sign = 0;

			while(n < (specifier - 1)) {
				if(buff_specifier[n] == '.') {
					end = n;
					n++;
					while(('0' <= buff_specifier[n] && buff_specifier[n] <= 9) 
					&& n < (specifier - 1)) {
						hold_number = hold_number == 0 ? (buff_specifier[n] -48) : hold_number * 10 +(buff_specifier[n] -48);
						n++;
					}
				}
				n++;
			}

			char flag = 'n';
			int width = 0;
			n = 0;
			int i = end == -1 ? (specifier - 1) : end;
			
			while(n < (i)) {
				if(buff_specifier[n] == '0') {
					flag = '0';
					n++;
					continue;
				}

				while(('0' <= buff_specifier[n] && buff_specifier[n] <= '9') && n <(i)) {
					width = width == 0 ? (buff_specifier[n] - 48) : width * 10 + (buff_specifier[n] -48);
					n++;
				}
				n++;
			}
			//decimal number
			if (*string == 'd') {
				string++;
				int x = va_arg(ap, int);
				int temp_index = MAX_PRINT_SIZE - 1;

				do {
					temp_buffer[temp_index] = (x % 10) + '0';
					temp_index--;
					x /= 10;
				} while(x != 0);

				for(int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
					buffer[buffer_index] = temp_buffer[i];
					buffer_index++;
				}
			}
			else if (*string == 'x') {
				//WRITE YOUR CODE HERE FOR HEXA FORMAT
				//Then continue with other formats
			}
		}
		else {
			buffer[buffer_index] = *string;
			buffer_index++;
			string++;
		}

		if (buffer_index == MAX_PRINT_SIZE - 1)
			break;
	}

	va_end(ap);


	//Print out formated string
	uart_puts(buffer);
}
