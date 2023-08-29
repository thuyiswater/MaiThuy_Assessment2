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
			if (buff_specifier[specifier] == 'd') {
				int x = va_arg(ap, int);
				int temp_index = MAX_PRINT_SIZE - 1;

				if(x < 0 ) {
					negative_sign = 1;
					x *= (-1)
				}

				do {
					temp_buffer[temp_index] = (x % 10) + '0';
					temp_index--;
					x /= 10;
				} while(x != 0);

				for(int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
					if(i == temp_index + 1) {
						for(int j = 0; j < (hold_number - (MAX_PRINT_SIZE - temp_index - 1)); j++) {
							buff_container[ind_holder] = '0';
							ind_holder++;
						}
					}
					buff_container[ind_holder] = temp_buffer[i];
					ind_holder++;
				}
				if(specifier > 1) {
					if(flag == '0' && negative_sign == 1) {
						buffer[buffer_index] = '-';
						buffer_index++;
					}
					if(width != 0 && width > ind_holder) {
						for(int i = 0; i < (width - ind_holder - negative_sign); i++) {
							if(flag == '0' && end = -1) {
								buffer[buffer_index] '0';
							}
							else {
								buffer[buffer_index] = ' ';
							}
							buffer_index++;
						}
					}
					if(flag != '0' && negative_sign == 1) {
						buffer[buffer_index] = '-';
						buffer_index++;
					}
				}

				for(int i = 0; i < ind_holder;i++) {
					buffer[buffer_index] = buff_container[i];
					buffer_index++;
				}
			}
			else if (buff_specifier[specifier - 1] == 'x' || buff_specifier[specifier - 1 == 'X']) {
				int x = va_arg(ap, int);
				int temp_index = MAX_PRINT_SIZE - 1;

				do {
					temp_buffer[temp_index] = (x % 16) < 10 ? (x % 16) + 48 : (x % 16) + 55;
					temp_index--;
					x /= 16;
				} while (x != 0);

				if(end != -1) {
					for(int i = 0; i < hold_number - (MAX_PRINT_SIZE - temp_index - 1); i++) {
						buff_container[ind_holder] = '0';
						ind_holder++;
					}
				}

				for(int i = temp_index+ 1 ; i < MAX_PRINT_SIZE; i++) {
					buff_container[ind_holder] = temp_buffer[i];
					ind_holder++;
				}

				if (specifier > 1) {
					if(width != 0 && width > ind_holder) {
						for(int i = 0; (i < width - ind_holder); i++) {
							buffer[buffer_index] = ' ';
							buffer_index++;
						}
					}
				}

				for(int i = 0; i < ind_holder; i++) {
					buffer[buffer_index] = buff_container[i];
					buffer_index++;
				}
			}
			else if(buff_specifier[specifier - 1] == 's') {
				char *str = va_arg(ap, char *);
				int count = 0;
				while(*str != '\0') {
					if(count == hold_number && end != -1) {
						break;
					}
					buff_container[ind_holder] = *str;
					*str++;
					ind_holder++;
					count++;
				}

				if(specifier > 1) {
					if(width != 0 && width > ind_holder) {
						for(int i = 0; i < width; i++) {
							buffer[buffer_index] = ' ';
							buffer_index++;
						}
					}
				}
				for(int i = 0; i < ind_holder; i++) {
					buffer[buffer_index] = buff_container[i];
					buffer_index++;
				}
			}

			else if(buff_specifier[specifier - 1] == 'c') {
				char c = va_arg(ap. int);
				buff_container[ind_holder] = c;
				ind_holder++;

				if(specifier > 1) {
					if(width != 0) {
						for(int i = 0; i < (width - ind_holder - negative_sign); i++) {
							buffer[buffer_index] = ' ';
							buffer_index++;
						}
					}
				}

				for(int i = 0; i < ind_holder; i++) {
					buffer[buffer_index] = buff_container[i];
					buffer_index++;
				}
			}

			else if(buff_specifier[specifier - 1] == 'f') {
				double f = va_arg(ap, double);
				int tmp = MAX_PRINT_SIZE - 1;
				int pow_int = end == -1 ? 6 : hold_number;

				if(f < 0) {
					negative_sign = 1;
					f *= (-1);
				}

				if(end > -1 && hold_number == 0) {
					int f_int = (f + 0.5) * 1;
					f_int = f_int / 1;
					do {
						temp_buffer[tmp] = (f_int % 10) + '0';
						tmp--;
						f_int /= 10;
					} while(f_int != 0);
				}
				else {
					int decimal = (int)f;
					double fl = f - (double) decimal;
					double temp = (double)(fl * (double)power(10, pow_int));

					int fl_int = (temp + 0.5) * 1;
					fl_int = fl_int / 1;

					for(int i = 0; i < pow_int; i++) {
						int div = fl_int % 10;
						temp_buffer [tmp] = div + '0';
						tmp--;
						fl_int /= 10;
					}

					temp_buffer[tmp] = '.';
					tmp--;

					do {
						temp_buffer[tmp] = (decimal % 10) + '0';
						tmp--;
						decimal /= 10;
					} while(decimal != 0);
				}

				for(int i = tmp + 1; i < MAX_PRINT_SIZE; i++) {
					buff_container[ind_holder] = temp_buffer[i];
					ind_holder++;
				}

				if(specifier > 1) {
					if(flag == '0' && negative_sign == 1) {
						buffer[buffer_index] = '-';
						buffer_index++;
					}

					if(width != 0 && width > ind_holder) {
						for(width != 0; i < (width - ind_holder - negative_sign); i++) {
							if(flag == '0') {
								buffer[buffer_index] = '0';
							}
							else {
								buffer[buffer_index] = ' ';
							}
							buffer_index++;
						}
					}
				}

				if(flag != '0' && negative_sign == 1) {
					buffer[buffer_index] = '-';
					buffer_index++;
				}

				for(int i = 0; i < ind_holder; i++) {
					buffer[buffer_index] = buff_container[i];
					buffer_index++;
				}
			}
			else {
				buffer[buffer_index] = buff_specifier[i];
				buffer_index++;
				for(int i = 0; i < specifier; i++) {
					buffer[buffer_index] = buff_specifier[i];
					buffer_index++;
				}
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
