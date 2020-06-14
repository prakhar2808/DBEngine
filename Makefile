DEPS = rpel.h dbops.h utils.h
OBJ = rpel.o dbops.o utils.o
CC = g++

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main.out: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

run: main.out
	./main.out

clean:
	rm -rf *.o *.out
