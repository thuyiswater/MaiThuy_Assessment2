// -----------------------------------main.c -------------------------------------

#include "uart.h"
#include "function.h"

#define MAX_CMD_SIZE 1024
void cli() {
    static char cli_buffer[MAX_CMD_SIZE];
    static int index = 0;
    char help[] = "help";

    //read and send back each char
    char c = uart_getc();
    uart_sendc(c);

    //put into a buffer until got new line character
    if (c != '\n'){
    cli_buffer[index] = c; //Store into the buffer
    index++;
    } else if (c == '\n'){
        cli_buffer[index] = '\0';
        uart_puts("\nGot commands: ");
        uart_puts(cli_buffer); 
        uart_puts("\n");

    /* Compare with supported commands and execute
    * ........................................... */
            if(compare_cli(cli_buffer,help) == 0) {
                // execute_command();
                uart_puts("Hi");
            }
    
    //Return to command line
    
        index = 0;
    }
}

void main(){
    // intitialize UART
    uart_init();
    welcome();
    // echo everything back 
    while(1) {
    	cli();
    }
}

