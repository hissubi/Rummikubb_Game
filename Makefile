all:
	g++ -g -Wall -O2 *.cpp -lncursesw -o rumi.out

clean:
	rm *.out
