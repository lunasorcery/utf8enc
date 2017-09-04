all: utf8enc

utf8enc: main.cpp
	$(CXX) main.cpp -std=c++0x -lstdc++ -o utf8enc

install: utf8enc
	cp utf8enc /usr/local/bin/utf8enc

clean:
	rm -f utf8enc
