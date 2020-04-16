#!/bin/bash

# grade.sh - Init Lab grader
rm -f /tmp/initlab_autograde /tmp/initlab_autograde_ltrace_q2 q3Output-05d448c2f2cd.txt &> /dev/null
touch /tmp/initlab_autograde &> /dev/null
touch /tmp/initlab_autograde_ltrace_q2 &> /dev/null
touch q3Output-05d448c2f2cd.txt &> /dev/null
(make &> /dev/null || (echo 'Error while compiling!' && exit 1)) && \

echo "Question 1 output:" && \
echo "------------------" && \

./initlab 1 && \

echo "" && \
echo "Question 2 output:" && \
echo "------------------" && \
ltrace -s 200 -o /tmp/initlab_autograde_ltrace_q2 -f ./initlab 2 && \

echo "" && \
echo "Question 3 output:" && \
echo "------------------" && \
./initlab 3 && \

echo "" && \
echo "Question 4 output:" && \
echo "------------------" && \
./initlab 4 && \

./initlab 0 &> /dev/null && \

python3 grader/grade.py /tmp/initlab_autograde grader/params.yml /tmp/initlab_autograde_ltrace_q2 handout && \
rm -f /tmp/initlab_autograde /tmp/initlab_autograde_ltrace_q2 q3Output-05d448c2f2cd.txt &> /dev/null

exit