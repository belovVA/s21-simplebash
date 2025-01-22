#!/ bin / bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
echo "" > ../test/log_cat.txt

TEST_FILE="../test/rand.txt"
for var in -b -e -n -s -t -v 
do
  VARCAT="$var"
  TEST_STROKE="$var $TEST_FILE"
  echo "$TEST_STROKE"
  ../cat/s21_cat $TEST_STROKE > s21_cat.txt
  TEST_STROKE_CAT="$VARCAT $TEST_FILE"
  cat $TEST_STROKE_CAT > cat.txt
  DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
  if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
    then
      (( COUNTER_SUCCESS++ ))
    else
      echo "$TEST_STROKE" >> ../test/log_cat.txt
    
      (( COUNTER_FAIL++ ))
  fi
  rm s21_cat.txt cat.txt
  for var2 in -b -e -n -s -t -v 
  do
    if [ $var != $var2 ]
        then
          VARCAT2="$var2"
          TEST_STROKE="$var $var2 $TEST_FILE"
          echo "$TEST_STROKE"
          ../cat/s21_cat $TEST_STROKE > s21_cat.txt
          TEST_STROKE_CAT="$VARCAT $VARCAT2 $TEST_FILE"
          cat $TEST_STROKE_CAT > cat.txt
          DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
          if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST_STROKE" >> ../test/log_cat.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_cat.txt cat.txt
        fi
    for var3 in -b -e -n -s -t -v 
    do
    if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
        then
          VARCAT3="$var3"
          TEST_STROKE="$var $var2 $var3 $TEST_FILE"
          echo "$TEST_STROKE"
          ../cat/s21_cat $TEST_STROKE > s21_cat.txt
          TEST_STROKE_CAT="$VARCAT $VARCAT2 $VARCAT3 $TEST_FILE"
          cat $TEST_STROKE_CAT > cat.txt
          DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
          if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST_STROKE" >> ../test/log_cat.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_cat.txt cat.txt

        fi
      for var4 in -b -e -n -s -t -v
        do
        if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ] && [ $var != $var4 ] && [ $var2 != $var4 ] && [ $var3 != $var4 ]
            then
              VARCAT4="$var4"
              TEST_STROKE="$var $var2 $var3 $var4 $TEST_FILE"
              echo "$TEST_STROKE"
              ../cat/s21_cat $TEST_STROKE > s21_cat.txt
              TEST_STROKE_CAT="$VARCAT $VARCAT2 $VARCAT3 $VARCAT4 $TEST_FILE"
              cat $TEST_STROKE_CAT > cat.txt
              DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
              if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
                then
                  (( COUNTER_SUCCESS++ ))
                else
                  echo "$TEST_STROKE" >> ../test/log_cat.txt
                  (( COUNTER_FAIL++ ))
              fi
              rm s21_cat.txt cat.txt

            fi
        for var5 in -e -t -v --number --number-nonblank --squeeze-blank
          do
          if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ] && [ $var != $var4 ] && [ $var2 != $var4 ] && [ $var3 != $var4 ] && [ $var != $var5 ] && [ $var2 != $var5 ] && [ $var3 != $var5 ]  && [ $var4 != $var5 ]
            then
              VARCAT5="$var5"
              TEST_STROKE="$var $var2 $var3 $var4 $var5 $TEST_FILE"
              echo "$TEST_STROKE"
              ../cat/s21_cat $TEST_STROKE > s21_cat.txt
              case "$var5" in
              --number ) VARCAT5="-n";;
              --number-nonblank ) VARCAT5="-b";;
              --squeeze-blank ) VARCAT5="-s";;
              esac
              TEST_STROKE_CAT="$VARCAT $VARCAT2 $VARCAT3 $VARCAT4 $VARCAT5 $TEST_FILE"
              cat $TEST_STROKE_CAT > cat.txt
              DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
              if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
                then
                  (( COUNTER_SUCCESS++ ))
                else
                  echo "$TEST_STROKE" >> ../test/log_cat.txt
                  (( COUNTER_FAIL++ ))
              fi
              rm s21_cat.txt cat.txt

            fi
        done
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

for file in ../test/test1.txt ../test/rand.txt
do
  TEST_FILE="$file"
  for var in -b -e -n -s -t -v 
  do
    for var2 in -b -e -n -s -t -v 
    do
        if [ $var != $var2 ]
        then
          TEST_STROKE="$var $var2 $TEST_FILE"
          echo "$TEST_STROKE"
          
          # Запуск через Valgrind и сохранение вывода
          valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind_output.txt ../cat/s21_cat $TEST_STROKE > s21_cat.txt

          
          
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
          
          rm s21_cat.txt  valgrind_output.txt
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