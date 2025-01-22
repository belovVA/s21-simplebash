#include <regex.h>

#include "../common/commonFunctions.h"

#define N 10

typedef struct {
  bool flag_e;
  bool flag_i;
  bool flag_v;
  bool flag_c;
  bool flag_l;
  bool flag_n;
  bool flag_h;
  bool flag_s;
  bool flag_f;
  bool flag_o;
  int reg_flags;
  int counterCons;
} grepShFlags;

typedef struct {
  int index4FS;
  int index4SS;
  int MaxSizeFS;
  bool flagOkMemory;
  bool flagEorFexsist;
} CopyStringParams;

void search_in_file(Date grepDates, grepShFlags *ShortFlags);
int creating_stroke_to_regular_expression(Date *Date,
                                          bool *_flagPatternInFlags);
int memory_allocation(char **stroke, int size);
void add_date_to_struct(Date *Date, grepShFlags *_ShortFlags);

int copy_strings(char **finalString, const char *secondString,
                 CopyStringParams *params);

int add_to_pattern_from_file(char **finalString, char *_tempPath,
                             CopyStringParams *params);

void grep_print_lines(int _res, grepShFlags *ShortFlags, Date *grepDates,
                      int _counterLine, char *_tempstr);

void grep_logic_flags_for_lines(grepShFlags *ShortFlags, Date *grepDates,
                                int _counterLine, char *_tempstr);
void grep_print_counters(grepShFlags *ShortFlags, Date grepDates);
void grep_flagO_logic(grepShFlags *ShortFlags, char *_pattern, char *_tempstr,
                      int _counterLine);
void main_processing(Date grepDates, int flagPatternInFlagsExsist);