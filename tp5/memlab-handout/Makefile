all:
	gcc -pthread -O0 -std=gnu11 -Wall -Wno-unused-result -Wno-unused-variable -o memlab grader/libgradememsim.o libmemlab.o memsim.c memlab.c -Wl,-ldl -Wl,-lm

handin:
	mkdir handin
	cp memsim.c handin/
	cp memsim.h handin/
	tar -czvf handin.tar.gz handin
	rm -rf handin

clean:
	rm -f memlab
	rm -f handin.tar.gz
