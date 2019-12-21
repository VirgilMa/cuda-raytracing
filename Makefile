SRC=src/main.cpp

run: clean build
	./out/demo

build: 
	mkdir out
	g++ $(SRC) -O2 -o ./out/demo

clean:
	rm -rf out
