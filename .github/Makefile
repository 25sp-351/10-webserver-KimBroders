CC      = clang
CFLAGS  = -g -Wall
RM      = rm -f


default: all

all: main.exe

main.exe: main.c handle_connection.h handle_connection.c request.h request.c path.h path.c
	$(CC) $(CFLAGS) -o main.exe main.c handle_connection.c request.c path.c
	
	

clean veryclean:
	$(RM) *.exe
	$(RM) *.ilk
	$(RM) *.pdb
	$(RM) *.o

debug: main.c heap.c
	$(CC) $(CFLAGS) -o main.exe main.c -DDEBUG

test: main.exe
	./main.exe -v -p 8886