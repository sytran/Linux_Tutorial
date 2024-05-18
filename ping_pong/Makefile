CC = gcc
SRC += ping_pong.c
TARGET = ping_pong

INCLUDEDIR += 
LIBRARYDIR +=  
LIBRARY += 

INCDIR = $(patsubst %,-I%,$(INCLUDEDIR))
LIBDIR = $(patsubst %,-L%,$(LIBRARYDIR))
LIB = $(patsubst %,-l%,$(LIBRARY))


OPT = -O0
DEBUG = -g
WARN= -Wall
PTHREAD= -pthread
	
all:
	$(CC) $(OPT) $(DEBUG) $(WARN) $(PTHREAD) $(INCDIR) $(LIBDIR) $(LIB) $(SRC) -o $(TARGET)

clean:
	rm -rf $(TARGET)



