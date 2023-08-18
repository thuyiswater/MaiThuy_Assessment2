#include "uart.h"
#include "mbox.h"

void reset_str(char* ar) {
    while (*ar != '\n'){
        ar++;
    }
}

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
}

int compare_cli(char* str1, char* str2) {
    int flag = 0;

    while(*str1 != '\0' || *str2 != '\0') {
        if(*str1 == *str2) {
            str1++; 
            str2++;
        } else if ((*str1 == '\0' && *str2 != '\0') || (*str1 != '\0' && *str2 == '\0') || *str1 != *str2)  {
            flag = 1;
            break;
        }
    }
    reset_str(str1);
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

// int check_help(char* ar) {
//     char *list[] = {"help",
//     "help hwinfo",
//     "clear",
//     "setcolor",
//     "showinfo"};

//     for(int i = 0; i < 4; i++) {
//         if(compare_cli(ar, list[i]) == 0) {
//             return i;
//             break;
//         }
//     }
// }

// void help_info(int help_type) {
//     if(help_type == 0) {
//         uart_puts("Command: help\n");
//         uart_puts("Show brief information of all commands\n");
//         uart_puts("Example: thuyiswater> help\n");
//     } 
//     if(help_type == 1) {
//         uart_puts("Command: help <command_name>\n");
//         uart_puts("Show full information of the command\n");
//         uart_puts("Example: thuyiswater> help hwinfo\n");
//     } 
//     if(help_type == 2) {
//         uart_puts("Command: clear\n");
//         uart_puts("Clear screen (in our terminal it will scroll down to current position of the cursor).\n");
//         uart_puts("Example: thuyiswater> clear\n");
//     }
//     if(help_type == 3) {
//         uart_puts("Command: setcolor -t <text color> -b <background color>\n");
//         uart_puts("Set text color, and/or background color of the console to one of the following colors: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE\n");
//         uart_puts("Example: thuyiswater> setcolor -t yellow\n");
//         uart_puts("\t\tsetcolor -b yellow -t white\n");
//     }
//     if(help_type == 4) {
//         uart_puts("Command: showinfo\n");
//         uart_puts("Show board revision and board MAC address in correct format/ meaningful information\n");
//         uart_puts("Example: thuyiswater> showinfo\n");
//     }
// } 

int compare_color(char* str1, char* str2) {
    int i = 0;
    int flag = 0;
    while (*str2 != '\0') {
        if (*str1 == *str2) {
            i++; str1++; str2++;
        } else {
            flag = 1;
            str1 = str1 - i;
            break;
        }
    }
    return flag;
}

int get_colorset(char* str1) {
    str1++;
    int flag = 0;

    if(*str1 == '\n') {
        flag = -1;
        return flag;
    }

    if (*str1 == ' '){
        str1++;
        int color = -1;

        for(int i = 0; i <= 8; i++) {
            if (compare_color(str1,"black") == 0){
                color = 0;
            } else if(compare_color(str1, "red") == 0) {
                color = 1;
            } else if (compare_color(str1, "green") == 0) {
                color = 2;
            } else if (compare_color(str1, "yellow") == 0) {
                color = 3;
            } else if (compare_color(str1, "blue") == 0) {
                color = 4;
            } else if (compare_color(str1, "purple") == 0) {
                color = 5;
            } else if(compare_color(str1, "cyan") == 0) {
                color = 6;
            } else if(compare_color(str1, "white") == 0) {
                color = 7;
            }
        }
        return color;
    } else {
        flag = -1;
        return flag;
    }
}

int set_color (char* str) {
    int flag = -1;

    while(*str != '\0') {
        if(*str == '-') {
            str++;
            if(*str == 'b') {
                int color = get_colorset(str);
                if(color < 0) {
                    return -1;
                } else {
                    if(flag == -1) {
                        flag = 8;
                        flag = color * 10 + flag;
                    } else if (flag % 10 == 8) {
                        flag = flag - 8;
                        flag = flag + color;
                    }
                }
            } else if (*str == 't') {
                int color = get_colorset(str);
                if(color < 0) {
                    return -1;
                } else {
                    if(flag == -1) {
                        flag = 8;
                        flag = color * 10 + flag;
                    } else if (flag / 10 == 8) {
                        flag = flag - 80;
                        flag =  color * 10 + flag;
                    }
                }
            } else {
                flag = -1;
                break;
            }
        }
        str++;
    }
    return flag;
}
