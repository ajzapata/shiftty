CC = g++
CFLAGS = -g -O0 -std=c++14 -Wall -Wextra -Wno-unused-parameter -lcurl
PRGM = shiftty
HFILES = shiftty.h http.h shapeshift-api.h shapeshift-api-debug.h json/json.h \
json/json-forwards.h
CFILES = shiftty.cpp http.cpp shapeshift-api.cpp shapeshift-api-debug.cpp \
jsoncpp.cpp
OFILES = $(CFILES:.cpp=.o)

all: $(OFILES)
	$(CC) $(CFLAGS) -o $(PRGM) $(OFILES)

http.o: http.cpp
	$(CC) -c $(CFLAGS) -o http.o http.cpp

jsoncpp.o: jsoncpp.cpp
	$(CC) -c $(CFLAGS) -o jsoncpp.o jsoncpp.cpp

shapeshift-api.o: shapeshift-api.cpp
	$(CC) -c $(CFLAGS) -o shapeshift-api.o shapeshift-api.cpp

shapeshift-api-debug.o: shapeshift-api-debug.cpp
	$(CC) -c $(CFLAGS) -o shapeshift-api-debug.o shapeshift-api-debug.cpp

shiftty.o: shiftty.cpp
	$(CC) -c $(CFLAGS) -o shiftty.o shiftty.cpp

clean:
	rm -f $(PRGM) *.o

# check:
