#!/bin/bash

# Test code regression on generated packages.

PACKAGE=$1
PKG_PREFIX="pkg_"
GT_PACKAGE="./integration/gt_data/packages/"${PKG_PREFIX}${PACKAGE}
TEST_PACKAGE="./integration/test_data/packages/"${PKG_PREFIX}${PACKAGE}
TEST_FILES=".c .h"

test_results () 
{
    RESULT="$(diff "$GT_FILE" "$TEST_FILE")"
    if [ -n "$RESULT" ]; then
        echo "Package " $PACKAGE " test failed for " $TEST_FILE
        echo $RESULT
        cat $TEST_FILE
        diff "$GT_FILE" "$TEST_FILE" > $TEST_FILE.failed
    fi     
}

for i in $TEST_FILES; do
    GT_FILE=${GT_PACKAGE}${i}
    TEST_FILE=${TEST_PACKAGE}${i}
    if ! [ -f "$TEST_FILE" ]; then
        echo "Package " $PACKAGE " test failed for " $TEST_FILE
        echo "File " $TEST_FILE " don't exist." > $TEST_FILE.failed
    fi
    test_results
done

# Check for error on the generated files.
for i in $TEST_FILES; do
    TEST_FILE=${TEST_PACKAGE}${i}
    [ -f "$TEST_FILE.failed" ] && exit
    [ -f "$TEST_PACKAGE.failed" ] && exit
done

echo "Test passed" >> ./integration/test_data/packages/$PACKAGE.passed
