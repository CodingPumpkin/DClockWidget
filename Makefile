exec = widget
build:
	gcc -Wall -lSDL2 -std=c99 ./src/*.c -o ./$(exec)

run:
	./$(exec)

clean:
	rm ./$(exec)

delete:
	delete.sh
