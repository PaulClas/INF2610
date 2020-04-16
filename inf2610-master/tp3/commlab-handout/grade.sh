#!/bin/bash

# grade.sh - Sync Lab grader
rm -f /tmp/commlab_autograde &> /dev/null
touch /tmp/commlab_autograde &> /dev/null
touch /tmp/commlablab_autograde_strace_p3 &> /dev/null
touch /tmp/commlablab_autograde_output_p3 &> /dev/null
(make &> /dev/null || (echo 'Error while compiling!' && exit 1)) && \

echo "Puzzle 1 output:" && \
echo "----------------" && \

./commlab 1 && \

echo "" && \
echo "Puzzle 2 output:" && \
echo "----------------" && \

./commlab 2 && \

echo "" && \
echo "Puzzle 3 output:" && \
echo "----------------" && \

strace -f -s 300 -o /tmp/commlablab_autograde_strace_p3 ./commlab 3 && \

echo "" && \

echo "Testing puzzle 3:" && \
echo "-----------------" && \

NOREPORT=1 ./commlab 3 2> /dev/null > /tmp/commlablab_autograde_output_p3 && \
./commlab -3 && \

echo "" && \

python3 grader/grade.py /tmp/commlab_autograde /tmp/commlablab_autograde_strace_p3 handout && \
rm -f /tmp/commlab_autograde /tmp/commlablab_autograde_strace_p3 /tmp/commlablab_autograde_output_p3 &> /dev/null

exit