CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -lGL -lGLU -lglut

SOURCE = src/*.cpp
BIN = bin/main
ZIP = xkoprd00.zip

all: $(BIN)

$(BIN): $(SOURCE)
	$(CC) $(SOURCE) $(CFLAGS) -o $(BIN)

run:	exp1	exp2	exp3

debug:
	$(CC) $(SOURCE) $(CFLAGS) -DDEBUG -g -o $(BIN)

zip:
	zip -j $(ZIP) src/* Makefile

clean:
	rm $(BIN)

exp1:
	./bin/main -p 80000 -i 2 -t 60 -c 600 -d 30 -q 

exp2:
	./bin/main -p 80000 -i 2 -t 60 -c 600 -d -1 -q

exp3:
	./bin/main -p 80000 -i 5 -t 60 -c 600 -d 14 -q
