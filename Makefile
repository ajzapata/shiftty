CC = g++
CFLAGS = -std=c++14 -Wall -Wextra -Wno-unused-parameter -lcurl
PRGM = shiftty
CFILES = main.cpp http.h http.cpp json.h json.cpp shapeshift-api.h \
shapeshift-api.cpp

# Debug
DDIR = bin/Debug
DCFLAGS = -g -O0

# Release
RDIR = bin/Release
RCFLAGS = -O2

DISTFILES = Makefile LICENSE README $(CFILES)

Debug: main.cpp
	$(CC) $(DCFLAGS) $(CFLAGS) -o $(DDIR)/$(PRGM) $(CFILES)

Release: main.cpp
	$(CC) $(RCFLAGS) $(CFLAGS) -o $(RDIR)/$(PRGM) $(CFILES)

tarball: $(DISTFILES)
	tar -zcvf $(PRGM).tar.gz $(DISTFILES)

clean:
	rm -f obj/Debug/* obj/Release/* bin/Debug/$(PRGM) bin/Release/$(PRGM) \
$(PRGM).tar.gz

# check:
