#include "printf.h"
#include "uart.h"

#define MAX_PRINT_SIZE 256

// Reset buffer by filling it with null terminators
void rst_buff(char* ar){
	int i = 0;
	while(ar[i] != '\0') {
		ar[i] = '\0';
		i++;
	}
}

// Extract specifier from the format string
void container(int* i, char* str, char* ar){
	char specifiers[] = {'c', 'd', 's', 'x','%','f'};


	while(*str != '\n' && *str != ' '){
		str++;
		*ar = *str;
		ar++;
		*i = *i + 1;

		for(int i = 0 ; i < 6; i++){
			if(*str == specifiers[i]){
				return;
			}
		}
	}
}

// Calculate power of a number
int power(int base_num , int exp){
	int exponent = base_num;
	for (int i = 1; i < exp; i++){
		exponent *= base_num;
	}
	return exponent;
}

void printf(char *string,...) {

	va_list ap;
	va_start(ap, string);

	char buffer[MAX_PRINT_SIZE];
	int buffer_index = 0;
	char buff_container[MAX_PRINT_SIZE];
	char buff_tmp[MAX_PRINT_SIZE];

	char buff_specifier[MAX_PRINT_SIZE];

	rst_buff(buffer);
	rst_buff(buff_specifier);

	while(1) {
		if (*string == 0)
			break;

		if (*string == '%') {
			// Handle literal %% (single % character)
			if(*(string + 1) == '%'){
				string++;
				while(*string != ' ' && *string != '\0'){
					buffer[buffer_index] = *string;
					buffer_index++;
					string++;
				}
				continue;
			}
			int sp_index = 0 ;
			container(&sp_index, string, buff_specifier);
			string += (sp_index +1 );

			int formatted_ind = 0;
			int end = -1; 
			int hold_number = 0;
			int i = 0 ;
			int flag_nega = 0;

			// Extract decimal precision and width from specifier
			while(i < (sp_index - 1)){
				if(buff_specifier[i] == '.'){
					end = i;
					i++;
					while(('0' <= buff_specifier[i] && buff_specifier[i] <= '9')
						&& i < (sp_index - 1)){
						hold_number = hold_number ==0 ? (buff_specifier[i] - 48) : hold_number*10+(buff_specifier[i] - 48);
						i++;
					}
				}
				i++;
			}

			char flag = 'n';
			int width = 0;
			i = 0 ;

			int index = end == -1 ? (sp_index - 1) : end;
			// Extract width and 0 flag from specifier
			while (i < (index)){
				
				if (buff_specifier[i] == '0'){
					flag = '0';
					i++;
					continue;
				}
				while(('0' <= buff_specifier[i] && buff_specifier[i] <= '9')
					&& i < (index)){
					width = width == 0? (buff_specifier[i] - 48) : width*10+ (buff_specifier[i] - 48);
					i++;
				}
				i++;
			}
			
			// Handle %d specifier
			if (buff_specifier[sp_index - 1] == 'd') {
				int x = va_arg(ap, int);
				int temp_index = MAX_PRINT_SIZE - 1;

				if(x < 0){
					flag_nega = 1;
					x *= (-1);
				}

				// Convert decimal number to string
				do {
					buff_tmp[temp_index] = (x % 10) + '0';
					temp_index--;
					x /= 10;
				} while(x != 0);

				// Copy formatted number to the container
				for(int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
					
					if(i == temp_index + 1){
						for (int x = 0 ; x < (hold_number- (MAX_PRINT_SIZE - temp_index -1 )); x++){
							buff_container[formatted_ind] = '0';
							formatted_ind++;
						}
					}
					buff_container[formatted_ind] = buff_tmp[i];
					formatted_ind++;
				}

				// Handle width and negative flag
				if (sp_index > 1){
					if(flag == '0' && flag_nega == 1){
						buffer[buffer_index] ='-';
						buffer_index++;
					}

					if(width != 0 && width > formatted_ind){

						for(int w = 0 ; w < (width - formatted_ind - flag_nega); w++){
							if(flag == '0' && end == -1){ 
								buffer[buffer_index] ='0';
							}
							else{
								buffer[buffer_index] =' ';
							}
							buffer_index++;
						}
					}
					
					if(flag != '0' && flag_nega == 1){
						buffer[buffer_index] ='-';
						buffer_index++;
					}
				}

				// Copy formatted number from the container to the buffer
				for(int i = 0 ; i < formatted_ind; i++) {
					buffer[buffer_index] = buff_container[i];
					buffer_index++;
				}
			}
			// Handle %x specifier
			else if (buff_specifier[sp_index - 1]  == 'x'){
				int x = va_arg(ap, int);
				int temp_index = MAX_PRINT_SIZE - 1;

				// Convert integer to hexadecimal
				do {
					buff_tmp[temp_index] = (x % 16)<10 ? (x%16)+48 : (x%16)+55;
					temp_index--;
					x /= 16;
				} while(x != 0);

				// Add leading zeros for precision
				if(end != -1 ){
					for (int i = 0; i < hold_number-(MAX_PRINT_SIZE -temp_index  -1 ) ; i++){
						buff_container[formatted_ind] = '0';
						formatted_ind++;
					}
				}

				// Copy formatted hexadecimal number to the container
				for(int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
					buff_container[formatted_ind] = buff_tmp[i];
					formatted_ind++;
				}

				// Handle width
				if (sp_index > 1){
					if(width != 0 && width > formatted_ind){
						for (int w = 0 ; (w < width - formatted_ind); w++){
							buffer[buffer_index] =' ';
							buffer_index++;
						}
					}
				}

				 // Copy formatted number from the container to the buffer
				for(int i = 0 ; i < formatted_ind ; i++) {
					buffer[buffer_index] = buff_container[i];
					buffer_index++;
				}

			}
			// Handle %s specifier
			else if (buff_specifier[sp_index - 1]  == 's'){
				char* s = va_arg( ap,  char *  );
				int pre_counter = 0;
				// Copy characters from string up to precision or end
				while(*s != '\0'){
					if(pre_counter == hold_number && end != -1){
						break;
					}
					buff_container[formatted_ind] = *s;
					s++;
					formatted_ind++;
					pre_counter++;
				}

				// Handle width
				if (sp_index > 1){
					if(width != 0 && width > formatted_ind){
						for (int w = 0 ; w < width; w++){
							buffer[buffer_index] =' ';
							buffer_index++;
						}
					}
				}
				
				// Copy formatted characters from the container to the buffer
				for(int i = 0 ; i < formatted_ind; i++) {
					buffer[buffer_index] = buff_container[i];
					buffer_index++;
				}
			}

			// Handle %c specifier
			else if (buff_specifier[sp_index - 1]  == 'c'){
				char c = va_arg( ap, int );
				buff_container[formatted_ind] = c ;
				formatted_ind++;

				// Handle width
				if (sp_index > 1){
					if(width != 0){
						for (int w = 0 ; w < (width - formatted_ind - flag_nega); w++){
							buffer[buffer_index] =' ';
							buffer_index++;
						}
					}
				}

				// Copy formatted character from the container to the buffer
				for(int i = 0 ; i < formatted_ind; i++) {
					buffer[buffer_index] = buff_container[i];
					buffer_index++;
				}
			}

			// Handle %f specifier
			else if (buff_specifier[sp_index - 1]  == 'f') {
				double x = va_arg(ap, double);
				int temp_index = MAX_PRINT_SIZE - 1;
				int pow_int = end == -1 ? 6 : hold_number;

				if(x < 0){
					flag_nega = 1;
					x *= (-1);
				}

				// Handle different cases for formatting floating-point numbers
				if (end > -1 && hold_number == 0){
					int x_int = (x + 0.5)*1;
					x_int = x_int/1;
					do {
						buff_tmp[temp_index] = (x_int % 10) + '0';
						temp_index--;
						x_int /= 10;
					} while(x_int != 0);
				}
				else{
					int dec_1 = (int)x;

					
					double fl = x - (double) dec_1 ;

					double temp =(double)(fl * (double)power(10,pow_int));

					int fl_int = (temp + 0.5)*1;
					fl_int = fl_int/1;

					
					for (int i = 0;i < pow_int; i++){
						int division = fl_int % 10;
						buff_tmp[temp_index] = division + '0';
						temp_index--;
						fl_int /= 10;
					}

					buff_tmp[temp_index] = '.'; temp_index--;
					do {
						buff_tmp[temp_index] = (dec_1 % 10) + '0';
						temp_index--;
						dec_1 /= 10;
					} while(dec_1 != 0);
				}

				// Copy formatted floating-point number to the container
				for(int i = temp_index + 1; i < MAX_PRINT_SIZE; i++) {
					
					buff_container[formatted_ind] = buff_tmp[i];
					formatted_ind++;
				}

				// Handle width and negative flag
				if (sp_index > 1){
					if(flag == '0' && flag_nega == 1){
						buffer[buffer_index] ='-';
						buffer_index++;
					}

					if(width != 0 && width > formatted_ind){
						for(int w = 0 ; w < (width - formatted_ind - flag_nega); w++){
							if(flag == '0'){
								buffer[buffer_index] ='0';
							}
							else{
								buffer[buffer_index] =' ';
							}
							buffer_index++;
						}
					}
				}
				
				if(flag != '0' && flag_nega == 1){
					buffer[buffer_index] ='-';
					buffer_index++;
				}

				// Copy formatted number from the container to the buffer
				for(int i = 0 ; i < formatted_ind; i++) {
					buffer[buffer_index] = buff_container[i];
					buffer_index++;
				}
			}

			// Handle unknown specifier by appending it to the buffer
			else{
				buffer[buffer_index] = buff_specifier[i];
				buffer_index++;
				for(int i = 0; i < sp_index; i++){
					buffer[buffer_index] = buff_specifier[i];
					buffer_index++;
				}
			}
		}
		// Copy character to the buffer
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
