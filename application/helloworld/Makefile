

test:1.o 2.o
	g++ -o test 1.o 2.o

1.o:1.cpp
	g++ -c 1.cpp 

2.o:2.cpp
	g++ -c 2.cpp 

.PHONY: clean
clean:
	rm -rf 1.o 2.o test
