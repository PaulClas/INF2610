#!/bin/bash

# grade.sh - Mem Lab grader
rm -f /tmp/memlab_autograde &> /dev/null
touch /tmp/memlab_autograde &> /dev/null
(make &> /dev/null || (echo 'Error while compiling!' && exit 1)) && \

echo "Question 1 test output:" && \
echo "-----------------------" && \

./memlab -1 && \
echo "" && \
./memlab -2 && \

echo "" && \

echo "Question 2 output:" && \
echo "------------------" && \

./memlab 2 && \

echo "" && \

python3 grader/grade.py /tmp/memlab_autograde handout && \
rm -f /tmp/memlab_autograde &> /dev/null

exit