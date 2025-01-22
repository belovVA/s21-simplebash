#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 256

typedef struct {
  char **masFlags;
  char **masFiles;

  char *pattern;
  char *validFlags;
  int counterFiles;
  int counterFlags;
  int FlagNumberFiles;
  char *currentFile;
  const struct option *longFlags;

} Date;

int add_to_mas_flags(char *tempArgc, Date *Date);
int add_to_mas_files(char *tempArgc, Date *Date);
int parsing_stroke(int argc, char *argv[], Date *Date);
int check_file_exist(char *fileName);
void add_dop_memory(char **tempstr, bool *flagOKMemory, int *size);
void parsing_e_flag(char **temp, const struct option *_LongFlags,
                    int *_flagOkParsing, int _rez);
void parsing_f_flag(char **temp, int *_flagOkParsing, int _rez);
void copy_long_flags(char **temp, char *flagName);
void clear_space(Date *Date);