#!/bin/bash

# grade.sh - Sync Lab grader
rm -f /tmp/synclab_autograde /tmp/synclab_autograde_ltrace_installer &> /dev/null
touch /tmp/synclab_autograde &> /dev/null
(make &> /dev/null || (echo 'Error while compiling!' && exit 1)) && \

echo "Installer output:" && \
echo "-----------------" && \

(GRADE=1 ltrace -S -f -s 300 -o /tmp/synclab_autograde_ltrace_installer ./synclab 1 ||
(echo "First attempt failed, retrying..." &&
rm -f /tmp/synclab_autograde /tmp/synclab_autograde_ltrace_installer &> /dev/null &&
GRADE=1 ltrace -S -f -s 300 -o /tmp/synclab_autograde_ltrace_installer ./synclab 1 || 
(echo "Second attempt failed, retrying..." &&
rm -f /tmp/synclab_autograde /tmp/synclab_autograde_ltrace_installer &> /dev/null &&
GRADE=1 ltrace -S -f -s 300 -o /tmp/synclab_autograde_ltrace_installer ./synclab 1 || 
(echo "Third attempt failed, retrying..." &&
rm -f /tmp/synclab_autograde /tmp/synclab_autograde_ltrace_installer &> /dev/null &&
GRADE=1 ltrace -S -f -s 300 -o /tmp/synclab_autograde_ltrace_installer ./synclab 1 || 
(echo "Fourth attempt failed, retrying..." &&
rm -f /tmp/synclab_autograde /tmp/synclab_autograde_ltrace_installer &> /dev/null &&
GRADE=1 ltrace -S -f -s 300 -o /tmp/synclab_autograde_ltrace_installer ./synclab 1))))) && \

echo "" && \

echo "Testing installer:" && \
echo "------------------" && \

./synclab -1 && \
GRADE=1 ./synclab -2 && \

echo "" && \

python3 grader/grade.py /tmp/synclab_autograde /tmp/synclab_autograde_ltrace_installer handout && \
rm -f /tmp/synclab_autograde /tmp/synclab_autograde_ltrace_installer &> /dev/null

exit