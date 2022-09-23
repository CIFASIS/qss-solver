#!/bin/bash

MODEL=$1
GT_MODEL="./system/gt_data/"${MODEL}/
TEST_MODEL="./system/test_data/"${MODEL}/
TEST_MODEL_RESULT="./system/test_data/"${MODEL}/${MODEL}
TEST_FILES="$(ls ./system/gt_data/"${MODEL}/"*.dat)"

test_results () 
{
    RESULT="$(diff "$GT_FILE" "$TEST_FILE")"
    if [ -n "$RESULT" ]; then
        echo "Model " $MODEL " test failed for " $TEST_FILE
        echo $RESULT
        diff "$GT_FILE" "$TEST_FILE" > $TEST_FILE.failed
        BASE_NAME=$(basename $TEST_FILE)
        echo "Simulation failed for " $BASE_NAME >> $TEST_MODEL_RESULT.failed
    fi     
}

for i in $TEST_FILES; do
    OUTPUT_VAR_NAME=$(basename $i)
    GT_FILE=${GT_MODEL}${OUTPUT_VAR_NAME}
    TEST_FILE=${TEST_MODEL}${OUTPUT_VAR_NAME}
    if ! [ -f "$TEST_FILE" ]; then
        echo "Model " $MODEL " test failed for " $TEST_FILE
        echo "File " $TEST_FILE " don't exist." > $TEST_FILE.failed
        echo "File " $TEST_FILE " don't exist." >> $TEST_MODEL_RESULT.failed
    fi
    test_results
done

# Check for error on the generated files.
[ -f "$TEST_MODEL_RESULT.failed" ] && exit

echo "Test passed" >> $TEST_MODEL_RESULT.passed
