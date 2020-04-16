all:
	gcc -O0 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o q1 q1.c
	gcc -O0 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o initlab libinitlab.o q2/libq2.o q3/libq3.o q4/libq4.o q2.c q3.c q4.c initlab.c

handin:
	mkdir handin
	cp answers-q5.h q1.c q2.c q3.c q4.c handin/
	tar -czvf handin.tar.gz handin
	rm -rf handin

clean:
	rm -f initlab
	rm -f handin.tar.gz
	rm -f q1
