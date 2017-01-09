CC = g++
CFLAGS = -g -O0 -std=c++14 -Wall -Wextra -Wno-unused-parameter -lcurl
PRGM = shiftty
HFILES = http.h shapeshift-api.h logger.h shapeshift-api-debug.h json/json.h \
json/json-forwards.h
CFILES = main.cpp http.cpp shapeshift-api.cpp logger.cpp \
shapeshift-api-debug.cpp jsoncpp.cpp
OFILES = $(CFILES:.cpp=.o)

DISTFILES = Makefile LICENSE README $(HFILES) $(CFILES)

all: $(OFILES)
	$(CC) $(CFLAGS) -o $(PRGM) $(OFILES)

http.o: http.cpp
	$(CC) -c $(CFLAGS) -o http.o http.cpp

logger.o: logger.cpp
	$(CC) -c $(CFLAGS) -o logger.o logger.cpp

jsoncpp.o: jsoncpp.cpp
	$(CC) -c $(CFLAGS) -o jsoncpp.o jsoncpp.cpp

shapeshift-api.o: shapeshift-api.cpp
	$(CC) -c $(CFLAGS) -o shapeshift-api.o shapeshift-api.cpp

shapeshift-api-debug.o: shapeshift-api-debug.cpp
	$(CC) -c $(CFLAGS) -o shapeshift-api-debug.o shapeshift-api-debug.cpp

main.o: main.cpp
	$(CC) -c $(CFLAGS) -o main.o main.cpp

tarball: $(DISTFILES)
	tar -zcvf $(PRGM).tar.gz $(DISTFILES)

clean:
	rm -f $(PRGM) $(PRGM).tar.gz *.o

# check:
