#include "../common/commonFunctions.h"

typedef struct {
  bool flag_b;
  bool flag_e;
  bool flag_E;
  bool flag_n;
  bool flag_s;
  bool flagt;
  bool flag_T;
  bool flag_v;
  bool flag_number_nonblank;
  bool flag_number;
  bool flag_squeeze;
} catShFlags;

void add_date_to_struct(char **_masFlags, int _counterFlags,
                        catShFlags *ShortFlags);
void print_console_from_file(char **_masFiles, int _counterFiles,
                             catShFlags *ShortFlags);
void changing_symbol_and_print(catShFlags *catFlags, char ch);
void print_number_str(int *countStrokes);
void print_result(catShFlags *catFlags, FILE *myfile);