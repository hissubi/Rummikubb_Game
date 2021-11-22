all:
	g++ -g -Wall -O2 *.cpp -lncurses -o rumi.out

clean:
	rm *.out
