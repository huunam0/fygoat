#makefile by Tran Huu Nam, huunam0@gmail.com
CC=g++
GFLAG= -I.
MFLAG= -I/usr/local/mysql/include/mysql -I/usr/include/mysql
LFLAG= -L/usr/local/mysql/lib/mysql -L/usr/lib/mysql  -lmysqlclient

all: 
	$(CC) -o gtoday shtml.cpp gtoday.cpp $(GFLAG)
gtoday:	
	$(CC) -o gtoday shtml.cpp gtoday.cpp $(GFLAG) $(MFLAG) $(LFLAG)
gtoday2:	
	$(CC) -o gtoday2 shtml.cpp gtoday2.cpp $(GFLAG) $(MFLAG) $(LFLAG)
gtable:
	$(CC) -o gtable shtml.cpp gtable.cpp $(GFLAG) $(MFLAG) $(LFLAG)
gmatch: needed.h
	$(CC) -o gmatch shtml.cpp gmatch.cpp $(GFLAG) $(MFLAG) $(LFLAG)

clear:
	rm -f gtoday gtable gmatch
clean:
	rm -f gtoday gtable gmatch
