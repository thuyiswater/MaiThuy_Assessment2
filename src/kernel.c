#include "uart.h"
#include "function.h"
#include "mbox.h"
#include "printf.h"

#define INPUT_MAX_SIZE 256

void reset_arr(char* arr){
    for (int i = 0; i < INPUT_MAX_SIZE; i++) {
        arr[i] = '\0'; // Fill buffer with null terminators
    }
}

void main() {
    uart_init();
    welcome();
    uart_puts("\nthuyiswater> ");

    char cli_buffer[INPUT_MAX_SIZE];
    int index = 0;

    while (1) {
        char c = uart_getc();
        uart_sendc(c);

        if (c == '\b' || c == '\177') {  // Handle backspace and delete character
            if (index > 0) {
                index--;
                uart_sendc('\b');
                uart_sendc(' ');
                uart_sendc('\b');
            }
        }
        else if(c == '\t') {
            cli_buffer[index] = '\0';

            if((compare_cli(cli_buffer, "h") == 0) || 
            (compare_cli(cli_buffer, "he") == 0) || 
            (compare_cli(cli_buffer, "hel") == 0)) {
                copyString(cli_buffer, "help", INPUT_MAX_SIZE - 1);
                index = count_length("help");
                uart_puts("\n");
                uart_puts("thuyiswater> ");
                uart_puts(cli_buffer);
            }
            else if((compare_cli(cli_buffer, "c") == 0) || 
            (compare_cli(cli_buffer, "cl") == 0) || 
            (compare_cli(cli_buffer, "cle") == 0) ||
            (compare_cli(cli_buffer, "clea") == 0)) {
                copyString(cli_buffer, "clear", INPUT_MAX_SIZE - 1);
                index = count_length("clear");
                uart_puts("\n");
                uart_puts("thuyiswater> ");
                uart_puts(cli_buffer);
            }
            else if((compare_cli(cli_buffer, "s") == 0)) {
                uart_puts("\nThere are two option start with 's', please insert one or more character");
                uart_puts("\nthuyiswater> ");
            }
            else if((compare_cli(cli_buffer, "se") == 0) || 
            (compare_cli(cli_buffer, "set") == 0) ||
            (compare_cli(cli_buffer, "setc") == 0) ||
            (compare_cli(cli_buffer, "setco") == 0) ||
            (compare_cli(cli_buffer, "setcol") == 0) ||
            (compare_cli(cli_buffer, "setcolo") == 0)) {
                copyString(cli_buffer, "setcolor", INPUT_MAX_SIZE - 1);
                index = count_length("setcolor");
                uart_puts("\n");
                uart_puts("thuyiswater> ");
                uart_puts(cli_buffer);
            }
            else if((compare_cli(cli_buffer, "sh") == 0) || 
            (compare_cli(cli_buffer, "sho") == 0) || 
            (compare_cli(cli_buffer, "show") == 0) ||
            (compare_cli(cli_buffer, "showi") == 0) ||
            (compare_cli(cli_buffer, "showin") == 0) ||
            (compare_cli(cli_buffer, "showinf") == 0)) {
                copyString(cli_buffer, "showinfo", INPUT_MAX_SIZE - 1);
                index = count_length("showinfo");
                uart_puts("\n");
                uart_puts("thuyiswater> ");
                uart_puts(cli_buffer);
            }
            else if((compare_cli(cli_buffer, "p") == 0) || 
            (compare_cli(cli_buffer, "pr") == 0) || 
            (compare_cli(cli_buffer, "pri") == 0) ||
            (compare_cli(cli_buffer, "prin") == 0) ||
            (compare_cli(cli_buffer, "print") == 0)) {
                copyString(cli_buffer, "printf", INPUT_MAX_SIZE - 1);
                index = count_length("printf");
                uart_puts("\n");
                uart_puts("thuyiswater> ");
                uart_puts(cli_buffer);
            }
        }
        else if (c == '\n') {  // Handle newline (Enter)
            cli_buffer[index] = '\0';
            int n = 0;
            
            char help[] = "help", clear[] = "clear";
            
            if (compare_cli(cli_buffer, help) == 0) {
                execute_command();
            }
            else if(check_help(cli_buffer) >= 0) {
                help_info(check_help(cli_buffer));
            }
            else if(compare_cli(cli_buffer,clear) == 0) {
                uart_puts("\033[2J");
                uart_puts("\033[H");
            } 
            else if(compare_cli(cli_buffer,"showinfo") == 0) {
                board_revision();
                board_mac_address();
            }
            else if(compare_cli(cli_buffer, "printlist") == 0) {
                print_list();
            }
            else {
                cli_buffer[index] = c;
                if(compare_input_color(cli_buffer, "setcolor", &n) == 0) {
                int cl_index = set_color(cli_buffer);

                    if(cl_index >= 0) {
                        color_arr(cl_index);
                    } 
                    else {
                        reset_str(cli_buffer);
                    }
                }
                else {
					uart_puts("Wrong command");
				}
            }
            reset_arr(cli_buffer);
            uart_puts("\nthuyiswater> ");
            index = 0;

        } 
        else {  // Handle regular character input
            if (c >= 64 && c <= 91) //or if(str[i]>='A' && str[i]<='Z')
				      c += 32;
				//add input to the char
				cli_buffer[index]=c;
				index++;
        }
    }
}