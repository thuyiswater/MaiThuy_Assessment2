#include "uart.h"
#include "mbox.h"

void welcome() {
    uart_puts("d88888b d88888b d88888b d888888b .d888b.   j88D  .d888b.  .d88b.\n");
    uart_puts("88'     88'     88'     `~~88~~' VP  `8D  j8~88  88' `8D .8P  88.\n");
    uart_puts("88ooooo 88ooooo 88ooooo    88       odD' j8' 88  `V8o88' 88  d'88\n");  
    uart_puts("88~~~~~ 88~~~~~ 88~~~~~    88     .88'   V88888D    d8'  88 d' 88 \n");
    uart_puts("88.     88.     88.        88    j88.        88    d8'   `88  d8'\n");
    uart_puts("Y88888P Y88888P Y88888P    YP    888888D     VP   d8'     `Y88P'\n");  
    uart_puts("\n");                                                            
                                                                  
    uart_puts("d8888b.  .d8b.  d8888b. d88888b    .d88b.  .d8888.\n");              
    uart_puts("88  `8D d8' `8b 88  `8D 88'       .8P  Y8. 88'  YP\n");              
    uart_puts("88oooY' 88ooo88 88oobY' 88ooooo   88    88 `8bo.\n");                
    uart_puts("88~~~b. 88~~~88 88`8b   88~~~~~   88    88   `Y8b.\n");              
    uart_puts("88   8D 88   88 88 `88. 88.       `8b  d8' db   8D\n");             
    uart_puts("Y8888P' YP   YP 88   YD Y88888P    `Y88P'  `8888Y'\n");

    uart_puts("\tDeveloped by Thuy - s3877746\n\n");
    uart_puts("MyOS>");
}

int compare_cli(char* str1, char* str2) {
    int flag = 0;

    while(*str1 != '\0' || *str2 != '\0') {
        if(*str1 == *str2) {
            str1++; str2++;
        } else if ((*str1 == '\0' && *str2 != '\0') || (*str1 != '\0' && *str2 == '\0') || (*str1 != *str2))  {
            flag = 1;
            break;
        }
    }
    return flag;
}

void execute_command() {
    uart_puts("\n\n");
    uart_puts("help                      - Show brief information of all commands\n");
    uart_puts("                          - Example: MyOS> help\n");    
    uart_puts("help <command_name>       - Show full information of the command\n");
    uart_puts("                          - Example: MyOS> help hwinfo\n");
    uart_puts("clear                     - Clear screen (in our terminal it will scroll down to current position of the cursor).\n");
    uart_puts("                          - Example: MyOS> clear\n");
    uart_puts("setcolor                  - Set text color, and/or background color of the console to one of\n");
    uart_puts("                          the following colors: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE\n");
    uart_puts("-t <text color> -b <background color>\n");
    uart_puts("showinfo                  - Show board revision and board MAC address in correct format/ meaningful information\n");
    uart_puts("                          - Example: MyOS> showinfo");
}