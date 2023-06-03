all: build run 

build:
	g++ -o main main.cpp

run: build 
	./main