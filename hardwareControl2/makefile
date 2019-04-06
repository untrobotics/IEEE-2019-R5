CC=g++
CFLAGS=-Wno-psabi$(IDIR)
DEPS = include/pathfind.h include/json/json.h
OBJ = pathfind.o jsoncpp.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

pathfind: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm ./*.o
