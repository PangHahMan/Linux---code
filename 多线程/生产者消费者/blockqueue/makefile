.PHONY:thread
.PHONY:clean
cp:main.cc
	g++ -o $@ $^ -std=c++11 -lpthread

clean:
	rm -f cp

run:
	./cp