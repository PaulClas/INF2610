all:
	gcc -fPIC -pthread -O3 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o synclab libsynclab.o installer/libinstaller.o grader/libgradeinstaller.o synclab.c installer.c

handin:
	mkdir handin
	cp installer.c handin/
	tar -czvf handin.tar.gz handin
	rm -rf handin

clean:
	rm -f synclab
	rm -f handin.tar.gz
