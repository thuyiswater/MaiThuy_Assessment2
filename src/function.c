#include "uart.h"
#include "mbox.h"
#include "printf.h"

#define MAX_HISTORY_SIZE 10
#define MAX_COMMAND_LENGTH 50
#define MAX_INPUT_SIZE 256
// #define NUM_COMMAND 5
const char commands[][MAX_INPUT_SIZE] = {
        "help",
        "clear",
        "setcolor",
        "showinfo",
        "printf"
};
char history[MAX_HISTORY_SIZE][MAX_COMMAND_LENGTH];
int historyIndex = 0;
int historyCount = 0;

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

    while (*str1 != '\0' || *str2 != '\0') {
        if (*str1 == *str2) {
            str1++;
            str2++;
        } else if ((*str1 == '\0' && *str2 != '\0') || (*str1 != '\0' && *str2 == '\0') || *str1 != *str2) {
            flag = 1;
            break;
        }
    }
    reset_str(str1);  // Reset the pointer here
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
    uart_puts("                          - Example: MyOS> showinfo\n");
}

int check_help(char* ar) {
    int n = -1;
    char *list[] = {"help help",
    "help hwinfo",
    "help clear",
    "help setcolor",
    "help showinfo"};

    for(int i = 0; i < 4; i++) {
        if(compare_cli(ar, list[i]) == 0) {
            n = i;
            break;
        }
    }
    return n;
}

void help_info(int help_type) {
    if(help_type == 0) {
        uart_puts("\nCommand: help\n");
        uart_puts("Show brief information of all commands\n");
        uart_puts("Example: thuyiswater> help\n");
    } 
    if(help_type == 1) {
        uart_puts("\nCommand: help <command_name>\n");
        uart_puts("Show full information of the command\n");
        uart_puts("Example: thuyiswater> help hwinfo\n");
    } 
    if(help_type == 2) {
        uart_puts("\nCommand: clear\n");
        uart_puts("Clear screen (in our terminal it will scroll down to current position of the cursor).\n");
        uart_puts("Example: thuyiswater> clear\n");
    }
    if(help_type == 3) {
        uart_puts("\nCommand: setcolor -t <text color> -b <background color>\n");
        uart_puts("Set text color, and/or background color of the console to one of the following colors: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE\n");
        uart_puts("Example: thuyiswater> setcolor -t yellow\n");
        uart_puts("\t\tthuyiswater> setcolor -b yellow -t white\n");
    }
    if(help_type == 4) {
        uart_puts("\nCommand: showinfo\n");
        uart_puts("Show board revision and board MAC address in correct format/ meaningful information\n");
        uart_puts("Example: thuyiswater> showinfo\n");
    }
} 

int compare_input_color(char *str1, char *str2, int *clAr) {
    int flag = 0;
    while(*str2 != '\0') {
        if(*str1 == *str2) {
            *clAr = *clAr + 1;
            str1++; str2++;
        }
        else {
            flag = 2;
            reset_str(str1);
            *clAr = 0;
            break;
        }
    }
    return flag;
}

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
                        flag = flag * 10 + color;
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

void color_arr(int color_index) {
    char text[8][10] = {"\x1b[30m",
						"\x1b[31m",
						"\x1b[32m",
						"\x1b[33m",
                        "\x1b[34m",
                        "\x1b[35m",
                        "\x1b[36m",
                        "\x1b[37m"};
    
    char background[8][10] = {"\x1b[40m",
                                "\x1b[41m",
                                "\x1b[42m",
                                "\x1b[43m",
                                "\x1b[44m",
                                "\x1b[45m",
                                "\x1b[46m",
                                "\x1b[47m"};
    
    int text_index = color_index / 10;
    if(text_index != 8) {
        uart_puts(text[text_index]);
    }

    int background_index = color_index % 10;
    if(background_index != 8) {
        uart_puts(background[background_index]);
    }
}

char* copyString(char *destination, const char *source, int maxSize) {
    char *origin = destination;

    while(maxSize > 0 && *source != '\0') {
        *destination++ = *source++;
        maxSize--;
    }

    while(maxSize > 0) {
        *destination++ = '\0';
        maxSize--;
    }
    return origin;
}

int count_length(char *s) {
    int n = 0;
    while(s[n] != '\0') {
        n += 1;
    }
    return n;
}
// int compare_length(char *str1, char str *str2) {
//     int length1 = count_length(str1);
//     int length2 = count_length(str2);

//     int similar_len = length1 > length2 ? length2 : length1;
//     int match = 0;

//     for(int i = 0; i < similar_len; i++) {
//         if(str1[i] == str2) {
//             match++;
//         }
//     }
//     return (int) match / similar_len * 100.0;
// }

// int tab_completion(char *cmd) {
//     int n = 0;
//     int match = -1;

//     for(int i = 0; i < 5; i++) {
//         int compare_match = compare_length(cmd, commands[i]);
//         if(compare_match == 100) {
//             n = compare_match
//         }
//     }
// }

void addToHistory(const char *command) {
    if (historyCount < MAX_HISTORY_SIZE) {
        for (int i = 0; i < MAX_COMMAND_LENGTH; i++) {
            history[historyCount][i] = command[i];
            if (command[i] == '\0') {
                break;
            }
        }
        historyCount++;
    } else {
        for (int i = 0; i < MAX_HISTORY_SIZE - 1; i++) {
            for (int j = 0; j < MAX_COMMAND_LENGTH; j++) {
                history[i][j] = history[i + 1][j];
            }
        }
        for (int i = 0; i < MAX_COMMAND_LENGTH; i++) {
            history[MAX_HISTORY_SIZE - 1][i] = command[i];
            if (command[i] == '\0') {
                break;
            }
        }
    }
}

void print_list() {
   printf("\n");
		//Print integer

		printf("%d is higher than %d\n ", 6, 4);
		printf("Hello %0d\n", 16);

        //Print character
		printf("\nCharacter: %c\n", 'd');

		//print hex
		printf("\nHex number: %x\n", 921);
		printf("Hex number: %d\n", -156);
		printf("Hex number: %d\n", -46.67);

		//print float
		printf("\nFloat number: %f\n", 44667.00076004);

		//float and flag and precision and width
		printf("Float number: %10.3f\n", 67.5 );
		printf("Float number: %10.3f\n", -67.5 );

		//Print string and flag and width
		printf("\nString: %s\n", "thuyiswater");

		//Print hexadecimal and flag and width
		printf ("\nHex number: %x \n", 7562);

		//print %
		printf ("\nUsing %%f: %f\n", 1234.56789);
}