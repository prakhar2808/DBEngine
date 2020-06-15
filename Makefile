DEPS = rpel.h db.h dbops.h utils.h memtable.h memtableWrapper.h 
OBJ = rpel.o db.o dbops.o utils.o memtable.o memtableWrapper.cc
CC = g++

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main.out: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

run: main.out
	./main.out

clean:
	rm -rf *.o *.out
