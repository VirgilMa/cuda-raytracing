SRC=src/main.cpp

run: clean build
	d=$$(date +%s)\
	; ./out/demo >> ./out/p.ppm\
	&& echo "Rendering took $$(($$(date +%s)-d)) s"

build: 
	mkdir out
	g++ $(SRC) -o ./out/demo

clean:
	rm -rf out
