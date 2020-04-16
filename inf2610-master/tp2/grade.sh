#!/bin/bash

# grade.sh - Clone Lab grader
rm -f /tmp/clonelab_autograde /tmp/clonelab_autograde_strace_part2 __PART2_LTRACE_LOG_PATH__ &> /dev/null
touch /tmp/clonelab_autograde &> /dev/null
touch /tmp/clonelab_autograde_strace_part2 &> /dev/null
touch __PART2_LTRACE_LOG_PATH__ &> /dev/null
(make -f Makefile &> /dev/null || (echo 'Error while compiling!' && exit 1)) && \

echo "Part 1 output:" && \
echo "--------------" && \

./clonelab 1 && \

echo "" && \
echo "Part 2 output:" && \
echo "--------------" && \

strace -s 200 -o /tmp/clonelab_autograde_strace_part2 -v -f ./clonelab 2 && \
NOREPORT=1 ltrace -s 200 -o __PART2_LTRACE_LOG_PATH__ -f ./clonelab 2 2>&1 > /dev/null && \

echo "" && \

./clonelab 3 &> /dev/null && \
python3 grader/grade.py /tmp/clonelab_autograde grader/params.yml /tmp/clonelab_autograde_strace_part2 __PART2_LTRACE_LOG_PATH__ handout && \
rm -f part2Output.txt /tmp/clonelab_autograde /tmp/clonelab_autograde_strace_part2 __PART2_LTRACE_LOG_PATH__ &> /dev/null

exit