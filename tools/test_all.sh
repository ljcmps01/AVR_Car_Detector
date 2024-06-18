#!/bin/bash
#Script intended to be used by CI to test all test files

SCRIP_DIR=$(dirname "$0")
TEST_FILES=$(ls src/test)

for file in $TEST_FILES
do
    TRIMMED_FILE=${file//'.c'}
    echo "test ${TRIMMED_FILE}"
    make TEST=$TRIMMED_FILE
done
    