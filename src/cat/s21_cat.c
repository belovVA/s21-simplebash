#include "s21_cat.h"

int main(int argc, char *argv[]) {
  Date catDates = {0};
  struct option options[] = {{"number-nonblank", 0, 0, '1'},
                             {"number", 0, 0, '2'},
                             {"squeeze-blank", 0, 0, '3'},
                             {NULL, 0, NULL, 0}};
  catDates.longFlags = options;
  catDates.validFlags = "+beEvnstT";

  bool flagOkToPrint = 1;

  if (!parsing_stroke(argc, argv, &catDates)) {
    flagOkToPrint = false;
  }
  if (flagOkToPrint) {
    catShFlags ShortFlags = {0};
    add_date_to_struct(catDates.masFlags, catDates.counterFlags, &ShortFlags);
    print_console_from_file(catDates.masFiles, catDates.counterFiles,
                            &ShortFlags);
  }

  // Очищение памяти
  clear_space(&catDates);
  return 0;
}

// Функция вывода файла с флагами
void print_console_from_file(char **_masFiles, int _counterFiles,
                             catShFlags *catFlags) {
  for (int i = 0; i < _counterFiles; i++) {
    if (!check_file_exist(_masFiles[i])) {
      fprintf(stderr, "cat: %s: No such file or directory\n", _masFiles[i]);
    } else {
      FILE *myfile = fopen(_masFiles[i], "rb");
      print_result(catFlags, myfile);
      fclose(myfile);
    }
  }
}

void print_result(catShFlags *catFlags, FILE *myfile) {
  int countStrokes = 0;
  char ch;
  bool flagEmptyStroke = 0;
  char chPred = '\n';
  while (((ch = fgetc(myfile)) != EOF)) {
    if (chPred == '\n') {
      if (ch == '\n' && (catFlags->flag_s || catFlags->flag_squeeze)) {
        if (flagEmptyStroke) {
          continue;
        } else {
          flagEmptyStroke = 1;
        }
      } else if (ch != '\n') {
        flagEmptyStroke = 0;
      }
      if (catFlags->flag_b || catFlags->flag_number_nonblank) {
        if (ch != '\n') {
          print_number_str(&countStrokes);
        }
      } else if ((catFlags->flag_n || catFlags->flag_number)) {
        print_number_str(&countStrokes);
      }
    }
    chPred = ch;
    changing_symbol_and_print(catFlags, ch);
  }
}

void print_number_str(int *countStrokes) {
  (*countStrokes)++;
  printf("%6d\t", *countStrokes);
}

// Реализация вывода символа из файла
void changing_symbol_and_print(catShFlags *catFlags, char ch) {
  if ((catFlags->flag_e || catFlags->flagt || catFlags->flag_v) && ch != '\n' &&
      ch != '\t') {
    if (ch < 32) {
      printf("^");
      ch += 64;
    } else if (ch >= 127) {
      printf("^");
      ch -= 64;
    }
  } else if (ch == '\t' && (catFlags->flag_T || catFlags->flagt)) {
    printf("^");
    ch += 64;
  }
  if ((catFlags->flag_e || catFlags->flag_E) && ch == '\n') {
    printf("$");
  }
  printf("%c", ch);
}

//  Добавление флагов в структуру
void add_date_to_struct(char **_masFlags, int _counterFlags,
                        catShFlags *_ShortFlags) {
  for (int i = 0; i < _counterFlags; i++) {
    if (strcmp(_masFlags[i], "b") == 0)
      (*_ShortFlags).flag_b = true;
    else if (strcmp(_masFlags[i], "e") == 0)
      (*_ShortFlags).flag_e = true;
    else if (strcmp(_masFlags[i], "E") == 0)
      (*_ShortFlags).flag_E = true;
    else if (strcmp(_masFlags[i], "n") == 0)
      (*_ShortFlags).flag_n = true;
    else if (strcmp(_masFlags[i], "s") == 0)
      (*_ShortFlags).flag_s = true;
    else if (strcmp(_masFlags[i], "t") == 0)
      (*_ShortFlags).flagt = true;
    else if (strcmp(_masFlags[i], "T") == 0)
      (*_ShortFlags).flag_T = true;
    else if (strcmp(_masFlags[i], "v") == 0)
      (*_ShortFlags).flag_v = true;
    else if (strcmp(_masFlags[i], "--number-nonblank") == 0)
      (*_ShortFlags).flag_number_nonblank = true;
    else if (strcmp(_masFlags[i], "--number") == 0)
      (*_ShortFlags).flag_number = true;
    else if (strcmp(_masFlags[i], "--squeeze-blank") == 0)
      (*_ShortFlags).flag_squeeze = true;
  }
}
