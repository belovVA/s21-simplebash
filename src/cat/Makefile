CFLAGS = -Wall -Wextra -Werror
TARGET = s21_cat
FILES = s21_cat.c ../common/commonFunctions.c 
s21_cat:
	gcc $(CFLAGS) $(FILES) -o $(TARGET)

clean:
	rm -rf $(TARGET)

rebuild: clean s21_cat


clang:
	cp ../../materials/linters/.clang-format ./.clang-format
	clang-format --style=Google -i *.c *.h
	clang-format --style=Google -n *.c *.h 
	rm .clang-format

test: clean s21_cat
	bash ../test/test_cat.sh