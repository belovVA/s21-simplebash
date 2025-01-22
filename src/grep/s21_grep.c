#include "s21_grep.h"

int main(int argc, char *argv[]) {
  bool flagOkToPrint = 1;

  Date grepDates = {0};
  grepDates.validFlags = "e:ivclnhsf:o";
  grepDates.longFlags = NULL;

  if (!parsing_stroke(argc, argv, &grepDates)) {
    flagOkToPrint = 0;
  }

  if (flagOkToPrint) {
    bool flagPatternInFlagsExsist = 0;
    flagOkToPrint = creating_stroke_to_regular_expression(
        &grepDates, &flagPatternInFlagsExsist);
    if (flagOkToPrint) {
      main_processing(grepDates, flagPatternInFlagsExsist);
    }
  }

  clear_space(&grepDates);
  return 0;
}

void main_processing(Date grepDates, int flagPatternInFlagsExsist) {
  grepShFlags ShortFlags = {0};
  add_date_to_struct(&grepDates, &ShortFlags);

  if (grepDates.counterFiles - (1 - flagPatternInFlagsExsist) > 1) {
    grepDates.FlagNumberFiles = 1;
  }

  for (int i = 1 - flagPatternInFlagsExsist; i < grepDates.counterFiles; i++) {
    if (check_file_exist(grepDates.masFiles[i])) {
      grepDates.currentFile = grepDates.masFiles[i];
      search_in_file(grepDates, &ShortFlags);
    } else if (!ShortFlags.flag_s) {
      fprintf(stderr, "grep: %s: No such file or directory\n",
              grepDates.masFiles[i]);
    }
  }
}

void search_in_file(Date grepDates, grepShFlags *ShortFlags) {
  regex_t regex;
  regmatch_t match;
  regcomp(&regex, grepDates.pattern, ShortFlags->reg_flags);
  FILE *tempFile = fopen(grepDates.currentFile, "rb");
  bool flagOKMemory = 1;
  int indexStroke = 0, counterLine = 0, size = M;
  char ch;
  char *tempstr;
  flagOKMemory = memory_allocation(&tempstr, size);

  while ((ch = fgetc(tempFile)) != EOF && flagOKMemory) {
    if (indexStroke >= size) {
      add_dop_memory(&tempstr, &flagOKMemory, &size);
    }
    if (flagOKMemory) {
      tempstr[indexStroke] = ch;
      indexStroke++;
    }
    if (ch == '\n' && flagOKMemory) {
      tempstr[indexStroke] = '\0';
      counterLine++;
      int res = regexec(&regex, tempstr, 1, &match, 0);
      grep_print_lines(res, ShortFlags, &grepDates, counterLine, tempstr);
      indexStroke = 0;
      size = M;
      free(tempstr);
      flagOKMemory = memory_allocation(&tempstr, size);
      tempstr[0] = '\0';
    }
  }

  if (tempstr[0] != '\0' && flagOKMemory) {
    tempstr[indexStroke] = '\n';
    tempstr[indexStroke + 1] = '\0';
    counterLine++;
    int res = regexec(&regex, tempstr, 1, &match, 0);
    grep_print_lines(res, ShortFlags, &grepDates, counterLine, tempstr);
  }
  if (flagOKMemory) grep_print_counters(ShortFlags, grepDates);
  free(tempstr);
  regfree(&regex);
}

// Создание строки для регулярного выражения
int creating_stroke_to_regular_expression(Date *Date,
                                          bool *_flagPatternInFlags) {
  bool flagOkPattern, flagEorFexsist = 0;
  int size = M, k = 0;
  flagOkPattern = memory_allocation(&Date->pattern, size);

  for (int i = 0; i < Date->counterFlags && flagOkPattern; i++) {
    if (Date->masFlags[i][0] == 'e') {
      CopyStringParams cpyParams = {k, 1, size, flagOkPattern, flagEorFexsist};
      k = copy_strings(&Date->pattern, Date->masFlags[i], &cpyParams);
      flagEorFexsist = 1;
      flagOkPattern = cpyParams.flagOkMemory;

    } else if (Date->masFlags[i][0] == 'f') {
      char *tempPath;
      CopyStringParams cpyPathParams = {0, 1, size, flagOkPattern, false};
      flagOkPattern = memory_allocation(&tempPath, size);
      copy_strings(&tempPath, Date->masFlags[i], &cpyPathParams);
      flagOkPattern = cpyPathParams.flagOkMemory;

      if (check_file_exist(tempPath) && flagOkPattern) {
        CopyStringParams cpyFileParams = {k, 0, size, flagOkPattern,
                                          flagEorFexsist};
        k = add_to_pattern_from_file(&Date->pattern, tempPath, &cpyFileParams);
        flagOkPattern = cpyFileParams.flagOkMemory;
      } else {
        printf("grep: %s: No such file or directory\n", tempPath);
      }
      flagEorFexsist = 1;
      if (tempPath) free(tempPath);
    }
  }

  if (flagOkPattern) {
    if (k == 0 && Date->counterFiles <= 1) {
      flagOkPattern = 0;
    } else if (k == 0) {
      CopyStringParams cpyParams = {k, 0, size, flagOkPattern, flagEorFexsist};
      k = copy_strings(&Date->pattern, Date->masFiles[0], &cpyParams);
      flagOkPattern = cpyParams.flagOkMemory;

    } else
      *_flagPatternInFlags = 1;
    (Date->pattern)[k] = '\0';
  }
  return flagOkPattern;
}

// Заполнение структуры по массиву флагов
void add_date_to_struct(Date *Date, grepShFlags *_ShortFlags) {
  for (int i = 0; i < Date->counterFlags; i++) {
    char tempChar = Date->masFlags[i][0];
    if (tempChar == 'e') {
      (*_ShortFlags).reg_flags |= REG_EXTENDED;
      (*_ShortFlags).flag_e = 1;
    } else if (tempChar == 'i') {
      (*_ShortFlags).reg_flags |= REG_ICASE;
      (*_ShortFlags).flag_i = 1;
    } else if (tempChar == 'v') {
      (*_ShortFlags).flag_v = 1;
    } else if (tempChar == 'c') {
      (*_ShortFlags).flag_c = 1;
    } else if (tempChar == 'l') {
      (*_ShortFlags).flag_l = 1;
    } else if (tempChar == 'n') {
      (*_ShortFlags).flag_n = 1;
    } else if (tempChar == 'h') {
      (*_ShortFlags).flag_h = 1;
    } else if (tempChar == 'f') {
      (*_ShortFlags).reg_flags |= REG_EXTENDED;
      (*_ShortFlags).flag_f = 1;
    } else if (tempChar == 'o') {
      (*_ShortFlags).flag_o = 1;
    } else if (tempChar == 's') {
      (*_ShortFlags).flag_s = 1;
    }
  }
}

// Копирование строк
// Возвращаемое значение - индекс следующего char в финальной строке
int copy_strings(char **finalString, const char *secondString,
                 CopyStringParams *params) {
  int index4FS = params->index4FS;
  int index4SS = params->index4SS;
  // добавление логического или, если паттерн уже есть
  if (params->flagEorFexsist) {
    (*finalString)[index4FS] = '|';
    index4FS++;
  }

  while (secondString[index4SS] != '\0') {
    (*finalString)[index4FS] = secondString[index4SS];
    index4FS++;
    index4SS++;
    if (index4FS == params->MaxSizeFS) {
      add_dop_memory(finalString, &(params->flagOkMemory),
                     &(params->MaxSizeFS));
    }
  }
  (*finalString)[index4FS] = '\0';
  return index4FS;
}

// посимвольное чтение из файла и добавление в паттерн
// возвращает индекc на новый char pattern'a
int add_to_pattern_from_file(char **finalString, char *_tempPath,
                             CopyStringParams *params) {
  int index4FS = params->index4FS;
  if (params->flagEorFexsist) {
    (*finalString)[index4FS] = '|';
    index4FS++;
  }
  FILE *tempFile = fopen(_tempPath, "rb");
  char ch, Predch = '\n';
  while (((ch = fgetc(tempFile)) != EOF)) {
    if (index4FS == (params->MaxSizeFS) && params->flagOkMemory) {
      (params->MaxSizeFS) += M;
      (*finalString) =
          (char *)realloc((*finalString), (params->MaxSizeFS) * sizeof(char));
    }
    if (Predch == '\n' && ch != '\n' && index4FS != 0 &&
        !params->flagEorFexsist) {
      (*finalString)[index4FS] = '|';
      index4FS++;
    }
    if (Predch == '\n' && ch == '\n') {
      (*finalString)[index4FS] = '|';
      index4FS++;
    }
    if (ch != '\n') {
      (*finalString)[index4FS] = ch;
      index4FS++;
    }
    Predch = ch;
    params->flagEorFexsist = 0;
  }

  fclose(tempFile);
  return index4FS;
}

// Выделение изначальной памяти;
int memory_allocation(char **stroke, int size) {
  int flagMemoryAllocated = 1;
  (*stroke) = (char *)malloc(size * sizeof(char));
  if ((*stroke) == NULL) {
    flagMemoryAllocated = 0;
  }
  return flagMemoryAllocated;
}

void grep_print_lines(int _res, grepShFlags *ShortFlags, Date *grepDates,
                      int _counterLine, char *_tempstr) {
  if ((ShortFlags->flag_v + _res) % 2 == 0) {
    (ShortFlags->counterCons)++;
    grep_logic_flags_for_lines(ShortFlags, grepDates, _counterLine, _tempstr);
  }
}

void grep_logic_flags_for_lines(grepShFlags *ShortFlags, Date *grepDates,
                                int _counterLine, char *_tempstr) {
  if (!(ShortFlags->flag_c || ShortFlags->flag_l)) {
    if (!ShortFlags->flag_h && grepDates->FlagNumberFiles) {
      printf("%s:", grepDates->currentFile);
    }
    if (ShortFlags->flag_n && !ShortFlags->flag_o) {
      printf("%d:", _counterLine);
    }
    if (ShortFlags->flag_o && !ShortFlags->flag_v) {
      grep_flagO_logic(ShortFlags, grepDates->pattern, _tempstr, _counterLine);
    } else {
      if (!ShortFlags->flag_o || !ShortFlags->flag_v) printf("%s", _tempstr);
    }
  }
}

void grep_print_counters(grepShFlags *ShortFlags, Date grepDates) {
  if (ShortFlags->flag_c) {
    if (!ShortFlags->flag_h && grepDates.FlagNumberFiles) {
      printf("%s:", grepDates.currentFile);
    }
    if (!ShortFlags->flag_l) {
      printf("%d\n", ShortFlags->counterCons);
    }
  }
  if (ShortFlags->flag_l && (ShortFlags->counterCons > 0)) {
    printf("%s\n", grepDates.currentFile);
  }
}

void grep_flagO_logic(grepShFlags *ShortFlags, char *_pattern, char *_tempstr,
                      int _counterLine) {
  int indexTempstr = 0;
  regmatch_t match;
  regex_t regexO;
  regcomp(&regexO, _pattern, ShortFlags->reg_flags);
  while (regexec(&regexO, _tempstr + indexTempstr, 1, &match, 0) == 0) {
    if (ShortFlags->flag_n) {
      printf("%d:", _counterLine);
    }
    printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
           _tempstr + indexTempstr + match.rm_so);
    indexTempstr += match.rm_eo;

    if (match.rm_so == -1) {
      break;
    }
  }
  regfree(&regexO);
}
