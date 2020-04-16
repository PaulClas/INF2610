all:
	gcc -O3 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o commlab libcommlab.o grader/libgradep3.o commlab.c puzzle1.c puzzle2.c puzzle3.c

handin:
	mkdir handin
	cp puzzle1.c handin/
	cp puzzle2.c handin/
	cp puzzle3.c handin/
	tar -czvf handin.tar.gz handin
	rm -rf handin

clean:
	rm -f commlab
	rm -f handin.tar.gz
