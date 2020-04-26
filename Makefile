all: renderer
	./renderer

renderer: renderer.cpp libs.h init.cpp init.h
	g++ -Wall renderer.cpp init.cpp -o renderer -lGL -lglfw -lGLEW
