DB_CC_SOURCES = $(wildcard dbengine/*.cc serverCode/*.cc)
DB_HEADERS = $(wildcard dbengine/*.h serverCode/*.h)

CLIENT_CC_SOURCES = $(wildcard clientCode/*.cc)
CLIENT_HEADERS = $(wildcard clientCode/*.h)

DB_OBJS = ${DB_CC_SOURCES:.cc=.o}

CLIENT_OBJS = ${CLIENT_CC_SOURCES:.cc=.o}

CC = g++
CFLAGS = -std=c++14 -g -pthread

all: dbServer.out dbClient.out

%.o: %.cc ${DB_HEADERS} ${CLIENT_HEADERS}
	$(CC) -c -o $@ $< $(CFLAGS)

dbServer.out: ${DB_OBJS}
	$(CC) -o $@ $^ $(CFLAGS)

dbClient.out: ${CLIENT_OBJS}
	$(CC) -o $@ $^ $(CFLAGS)

run: dbServer.out dbClient.out
	./dbServer.out

clientrun: dbClient.out
	./dbClient.out

cleanSSTables:
	rm -rf SSTablesDir/* bloomFiltersDir/*
clean:
	rm -rf *.o *.out
	rm -rf dbengine/*.o serverCode/*.o clientCode/*.o
