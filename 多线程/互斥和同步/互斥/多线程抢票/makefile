.PHONY:thread
.PHONY:clean
thread:thread.cc
	g++ -o $@ $^ -std=c++11 -lpthread

clean:
	rm -f thread

run:
	./thread