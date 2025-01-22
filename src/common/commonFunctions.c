#include "commonFunctions.h"

int parsing_stroke(int argc, char *argv[], Date *Date) {
  int flagOkParsing = 1;
  char *temp;
  int rez;
  while ((rez = getopt_long(argc, argv, Date->validFlags, Date->longFlags,
                            0)) != -1) {
    switch (rez) {
      case 'e':
        parsing_e_flag(&temp, Date->longFlags, &flagOkParsing, rez);
        break;
      case 'f':
        parsing_f_flag(&temp, &flagOkParsing, rez);
        break;
      case '1':
        copy_long_flags(&temp, "--number-nonblank");
        break;
      case '2':
        copy_long_flags(&temp, "--number");
        break;
      case '3':
        copy_long_flags(&temp, "--squeeze-blank");
        break;
      case '?':
        flagOkParsing = 0;
        break;
      default:
        temp = (char *)malloc((strlen("a") + 1) * sizeof(char));
        temp[0] = rez;
        temp[1] = '\0';
        break;
    }
    if (flagOkParsing) {
      add_to_mas_flags(temp, Date);
      free(temp);
    }
  }

  if (flagOkParsing) {
    for (int i = optind; i < argc; i++) {
      add_to_mas_files(argv[i], Date);
    }
  }
  return flagOkParsing;
}

int add_to_mas_flags(char *tempArgc, Date *Date) {
  int flagOkParsing = 1;

  // Выделение места под указатель для массива char'ов следующегго флага
  Date->masFlags =
      realloc(Date->masFlags, (Date->counterFlags + 1) * sizeof(char *));
  if (Date->masFlags == NULL) {
    flagOkParsing = 0;

  } else {
    // Выделение памяти под массив char'ов для имени флага
    (Date->masFlags)[Date->counterFlags] =
        malloc((strlen(tempArgc) + 1) * sizeof(char));
    if ((Date->masFlags)[Date->counterFlags] == NULL) {
      flagOkParsing = 0;

    } else {
      strcpy((Date->masFlags)[Date->counterFlags], tempArgc);
      Date->counterFlags += 1;
    }
  }
  return flagOkParsing;
}

int add_to_mas_files(char *tempArgc, Date *Date) {
  int flagOkParsing = 1;

  Date->masFiles =
      realloc(Date->masFiles, (Date->counterFiles + 1) * sizeof(char *));
  if (Date->masFiles == NULL) {
    flagOkParsing = 0;

  } else {
    (Date->masFiles)[Date->counterFiles] =
        malloc((strlen(tempArgc) + 1) * sizeof(char));
    if ((Date->masFiles)[Date->counterFiles] == NULL) {
      flagOkParsing = 0;

    } else {
      strcpy((Date->masFiles)[Date->counterFiles], tempArgc);
      Date->counterFiles += 1;
    }
  }
  return flagOkParsing;
}

int check_file_exist(char *fileName) {
  int flagFileExists;
  FILE *myfile;
  myfile = fopen(fileName, "rи");
  if (myfile == NULL) {
    flagFileExists = 0;
  } else {
    flagFileExists = 1;
    fclose(myfile);
  }
  return flagFileExists;
}

void add_dop_memory(char **tempstr, bool *flagOKMemory, int *size) {
  *size += M;
  char *tmp = (char *)realloc((*tempstr), (*size) * sizeof(char));
  if (tmp == NULL) {
    (*flagOKMemory) = 0;
    free(*tempstr);
    free(tmp);
  } else {
    (*tempstr) = tmp;
  }
}

void parsing_e_flag(char **temp, const struct option *_LongFlags,
                    int *_flagOkParsing, int _rez) {
  if (_LongFlags == NULL) {
    (*temp) = (char *)malloc((strlen(optarg) + 2) * sizeof(char));
    if ((*temp) == NULL) {
      (*_flagOkParsing) = 0;
    } else {
      (*temp)[0] = _rez;
      int j = 1;
      for (int i = 0; i < (int)strlen(optarg); i++) {
        (*temp)[j] = optarg[i];
        j++;
      }
      (*temp)[strlen(optarg) + 1] = '\0';
    }

  } else {
    (*temp) = (char *)malloc((strlen("a") + 1) * sizeof(char));
    (*temp)[0] = _rez;
    (*temp)[1] = '\0';
  }
}

void parsing_f_flag(char **temp, int *_flagOkParsing, int _rez) {
  (*temp) = (char *)malloc((strlen(optarg) + 2) * sizeof(char));
  if ((*temp) == NULL) {
    (*_flagOkParsing) = 0;
  } else {
    (*temp)[0] = _rez;
    int j = 1;
    for (int i = 0; i < (int)strlen(optarg); i++) {
      (*temp)[j] = optarg[i];
      j++;
    }
    (*temp)[strlen(optarg) + 1] = '\0';
  }
}

void copy_long_flags(char **temp, char *flagName) {
  (*temp) = (char *)malloc((strlen(flagName) + 1));
  strcpy((*temp), flagName);
  (*temp)[strlen(flagName)] = '\0';
}

void clear_space(Date *Date) {
  free(Date->pattern);
  for (int i = 0; i < Date->counterFlags; i++) free(Date->masFlags[i]);
  for (int i = 0; i < Date->counterFiles; i++) free(Date->masFiles[i]);
  free(Date->masFlags);
  free(Date->masFiles);
}