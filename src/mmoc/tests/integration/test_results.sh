#/bin/bash

MODEL=$1
GT_MODEL="./integration/gt_data/"${MODEL}/${MODEL}
TEST_MODEL="./integration/test_data/"${MODEL}
TEST_FILES=".c .h .ini"

test_results () 
{
    RESULT="$(diff "$GT_FILE" "$TEST_FILE")"
    if [ -n "$RESULT" ]; then
        echo "Model " $MODEL " test failed for " $TEST_FILE
        diff "$GT_FILE" "$TEST_FILE" > $TEST_FILE.failed
    fi    
}

for i in $TEST_FILES; do
    GT_FILE=${GT_MODEL}${i}
    TEST_FILE=${TEST_MODEL}${i}
    test_results
done

# Check for error on the generated files.

for i in $TEST_FILES; do
    TEST_FILE=${TEST_MODEL}${i}
    [ -f "$TEST_FILE.failed" ] && exit
done

echo "Test passed" > ./integration/test_data/$MODEL.passed