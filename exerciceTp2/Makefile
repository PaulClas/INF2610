all: demo1 demo2 demo3

clean: clean-demo1 clean-demo2 clean-demo3

demo1: clean-demo1
	gcc -Werror -Wall -o demo1 demo1.c

clean-demo1:
	rm -f demo1

demo2: clean-demo2
	gcc -Werror -Wall -o demo2 demo2.c

clean-demo2:
	rm -f demo2

demo3: clean-demo3
	gcc -Wall -o demo3 demo3.c

clean-demo3:
	rm -f demo3