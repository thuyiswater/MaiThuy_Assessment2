#include "uart.h"
#include "function.h"
#include "mbox.h"

void main() {
    uart_init();
    welcome();
    uart_puts("\nthuyiswater> ");

    char cli_buffer[256];
    int index = 0;

    while (1) {
        
        char c = uart_getc();

        if (c == '\b' || c == '\177') {  // Handle backspace and delete character
            if (index > 0) {
                index--;
                uart_sendc('\b');
                uart_sendc(' ');
                uart_sendc('\b');
            }
        } else if (c == '\n') {  // Handle newline (Enter)
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
            } 
            else if(compare_cli(cli_buffer,"showinfo") == 0) {
                board_revision();
                board_mac_address();
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
            }
            uart_puts("\nthuyiswater> ");
        } 
        else {  // Handle regular character input
            if (index < sizeof(cli_buffer) - 1) {
                uart_sendc(c);
                cli_buffer[index++] = c;
            }
        }
    }
}
