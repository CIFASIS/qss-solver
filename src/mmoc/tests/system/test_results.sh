#!/bin/bash

MODEL=$1
MODEL_FUNCTIONS=$2
GT_MODEL="./system/gt_data/"${MODEL}/${MODEL}
TEST_MODEL="./system/test_data/"${MODEL}
TEST_FILES=".c .h .ini"
if [[ ${MODEL_FUNCTIONS} ]]
then
    TEST_FILES=$TEST_FILES" _functions.c"
fi

test_results () 
{
    RESULT="$(diff "$GT_FILE" "$TEST_FILE")"
    if [ -n "$RESULT" ]; then
        echo "Model " $MODEL " test failed for " $TEST_FILE
        echo $RESULT
        cat $TEST_FILE
        diff "$GT_FILE" "$TEST_FILE" > $TEST_FILE.failed
    fi     
}

# Check for compilation warnings
COMPILE=`wc -l ${TEST_MODEL}.log | awk '{ print $1 }'`
if ! [ "$COMPILE" = "1" ]; then
    echo "Compilation failed" > ${TEST_MODEL}.failed
fi

for i in $TEST_FILES; do
    GT_FILE=${GT_MODEL}${i}
    TEST_FILE=${TEST_MODEL}${i}
    if ! [ -f "$TEST_FILE" ]; then
        echo "Model " $MODEL " test failed for " $TEST_FILE
        echo "File " $TEST_FILE " don't exist." > $TEST_FILE.failed
    fi
    test_results
done

# Check for error on the generated files.
for i in $TEST_FILES; do
    TEST_FILE=${TEST_MODEL}${i}
    [ -f "$TEST_FILE.failed" ] && exit
    [ -f "$TEST_MODEL.failed" ] && exit
done

echo "Test passed" >> ./system/test_data/$MODEL.passed