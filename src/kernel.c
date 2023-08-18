// -----------------------------------main.c -------------------------------------

#include "uart.h"
#include "function.h"

#define MAX_CMD_SIZE 256
void main(){
    // intitialize UART
    uart_init();
    welcome();
    char cli_buffer[MAX_CMD_SIZE];
    int index = 0;
    // echo everything back 
    while(1) {
    	uart_puts("\nthuyiswater> ");

        char c = uart_getc();
        uart_sendc(c);

        char help[] = "help", clear[] = "clear";

        if(c == '\n') {
            cli_buffer[index] - '\0';
            index = 0;
            if(compare_cli(cli_buffer, help) == 0) {
                execute_command();
            } else if (check_help(cli_buffer) >= 0) {
                help_info(check_help(cli_buffer));
            } else if (compare_cli(cli_buffer,clear) == 0) {
                //clear screen
                uart_puts("\033[2J");
            }
        }
    }
}

