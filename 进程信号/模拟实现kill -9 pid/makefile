.PHONY:all
.PHONY:mykill
.PHONY:clean

all:mykill loop

loop:loop.cc
	g++ -o $@ $^ -std=c++11

mykill:mykill.cc
	g++ -o $@ $^ -std=c++11

clean:
	rm -f mykill loop

