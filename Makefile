all:
	g++ -g -Wall -O0 *.cpp -lncurses -o rumi.out

clean:
	rm *.out
