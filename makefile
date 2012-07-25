#makefile by Tran Huu Nam, huunam0@gmail.com
CC=g++
GFLAG= -I.
MFLAG= -I.

all: 
	$(CC) -o gtoday shtml.cpp gtoday.cpp $(GFLAG)
gtoday:	
	$(CC) -o gtoday shtml.cpp gtoday.cpp $(GFLAG)
gtable:
	$(CC) -o gtable shtml.cpp gtable.cpp $(GFLAG)
gmatch: needed.h
	$(CC) -o gmatch shtml.cpp fmatch.cpp gmatch.cpp $(GFLAG)

clear:
	rm -f gtoday gtable gmatch
clean:
	rm -f gtoday gtable gmatch
