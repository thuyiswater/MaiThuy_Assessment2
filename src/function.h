#ifndef SRC_FUNCTION_H_
#define SRC_FUNCTION_H_

char color[8][10] = {"black", "red", "green", "yellow", "blue", "purple", "cyan", "white"};
void reset_str(char *ar);
void welcome();
int compare_cli(char *str1, char *str2);
void execute_command();
int check_help(char* ar);
void help_info(int help_type);
int compare_input_color(char *str1, char *str2, int *clAr);
int compare_color(char *str1, char *str2);
int get_colorset(char *str1);
int set_color(char* str);
void color_arr(int color_index);

#endif
