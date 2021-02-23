main:main.o myhead.o
	g++ main.o myhead.o -o main -I ./myinclude/ 

main.o:./src/main.cpp
	g++ -c ./src/main.cpp -o main.o -I./myinclude/ 
myhead.o:./myinclude/myhead.cpp
	g++ -c ./myinclude/myhead.cpp -o myhead.o

.PHONY: clean
clean:
	rm -rf main main.o myhead.o