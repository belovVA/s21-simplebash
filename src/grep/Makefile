CFLAGS = -Wall -Wextra -Werror -g
TARGET = s21_grep
FILES = s21_grep.c  ../common/commonFunctions.c 
s21_grep:
	gcc $(CFLAGS) $(FILES) -o $(TARGET)

clean:
	rm -rf $(TARGET)

rebuild: clean s21_grep

clang:
	cp ../../materials/linters/.clang-format ./.clang-format
	clang-format --style=Google -i *.c *.h ../*/*.c ../*/*.h 
	clang-format --style=Google -n *.c *.h ../*/*.c ../*/*.h 
	rm .clang-format


test: clean s21_grep
	bash ../test/test_grep.sh
