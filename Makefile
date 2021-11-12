all:
	g++ -g -Wall -O0 *.cpp -o rumi.out

clean:
	rm *.out *.o
