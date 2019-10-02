all:
	gcc -fPIC -O0 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o clonelab libclonelab.o clonelab.c part2.c part1.c

handin:
	mkdir handin
	cp part1.c part2.c handin/
	tar -czvf handin.tar.gz handin
	rm -rf handin

clean:
	rm -f clonelab
	rm -f handin.tar.gz