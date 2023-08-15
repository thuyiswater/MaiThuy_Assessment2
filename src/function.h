#ifndef SRC_FUNCTION_H_
#define SRC_FUNCTION_H_

char color[8][10] = {"black", "red", "green", "yellow", "blue", "purple", "cyan", "white"};
void reset(char *ar);
void welcome();
void compare_cli();
void execute_command();
int compare_color(char *str1, char *str2);
int get_color(char *str1);

#endif
