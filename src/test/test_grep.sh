#!/ bin / bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
echo "" > ../test/log_grep.txt

for file in ../test/test1.txt ../test/test3.txt 
do
  TEST_FILE="$file"
  for var in -eca -i -v -c -l -n -h -s -fpatterns.txt -o
  do
    for var2 in -eca -i -v -c -l -n -h -s -fpatterns.txt -o
    do
      for var3 in -eca -i -v -c -l -n -h -s -fpatterns.txt -o
      do
        if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
        then
          VARGREP="$var"
          PATTERN="ca"
          
          for varS in $var $var2 $var3
          do
            case "$varS" in
            -eca ) PATTERN="";;
            -fpatterns.txt ) PATTERN="";;
            esac
          done
          TEST_STROKE="$var $var2 $var3 $PATTERN $TEST_FILE"
          echo "$TEST_STROKE"
          ../grep/s21_grep $TEST_STROKE > s21_grep.txt
          grep $TEST_STROKE > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST_STROKE" >> ../test/log_grep.txt
            
              (( COUNTER_FAIL++ ))
        fi
    rm s21_grep.txt grep.txt
          fi
      done
    done
  done
done



echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"


echo "Valgrind test started"
COUNTER_SUCCESS_VALGRIND=0
COUNTER_FAIL_VALGRIND=0
DIFF_RES=""
echo "" > ../test/valgrind_log.txt

for file in ../test/test1.txt
do
  TEST_FILE="$file"
  for var in -eca -i -v -c -l -n -h -s -fpatterns.txt -o
  do
    for var2 in -i -v -c -l -n -h -s
    do
        if [ $var != $var2 ]
        then
          VARGREP="$var"
          PATTERN="ca"
          
          for varS in $var $var2
          do
            case "$varS" in
            -eca ) PATTERN="";;
            -fpatterns.txt ) PATTERN="";;
            esac
          done
          TEST_STROKE="$var $var2 $PATTERN $TEST_FILE"
          echo "$TEST_STROKE"
          
          # Запуск через Valgrind и сохранение вывода
          valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind_output.txt ../grep/s21_grep $TEST_STROKE > s21_grep.txt

          
          
          # Проверка последней строки вывода Valgrind на наличие ошибок
          VALGRIND_ERRORS=$(tail -n 1 valgrind_output.txt)
          if [[ "$VALGRIND_ERRORS" != *"ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)"* ]]
          then
            (( COUNTER_FAIL_VALGRIND++ ))
            echo "$TEST_STROKE" >> ../test/valgrind_log.txt
            cat valgrind_output.txt >> ../test/valgrind_log.txt
            echo "" >> ../test/valgrind_log.txt
          else
            (( COUNTER_SUCCESS_VALGRIND++ ))
          fi
          
          rm s21_grep.txt  valgrind_output.txt
        fi
      done
    done
  done

echo "RESULT OF TESTS"
echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"
echo "--------------"
echo VALGRIND TESTS:
echo "SUCCESS: $COUNTER_SUCCESS_VALGRIND"
echo "FAIL: $COUNTER_FAIL_VALGRIND"
