SRC=src/main.cpp

run: clean build
	d=$$(date +%s)
	./out/demo >> ./out/p.ppm
	echo "Run took $$(($$(date +%s)-d)) ns"

build: 
	mkdir out
	g++ $(SRC) -o ./out/demo

clean:
	rm -rf out