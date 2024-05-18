CC = gcc
SRC += StateMc_pThread_Sem.c
TARGET = StateMc_pThread_Sem

LIBRARY += 
LIBRARYDIR +=  
XLINK_LIBDIR += 
INCLUDEDIR += 

INCDIR = $(patsubst %,-I%,$(INCLUDEDIR))
LIBDIR = $(patsubst %,-L%,$(LIBRARYDIR))
LIB = $(patsubst %,-l%,$(LIBRARY))
XLINKDIR = $(patsubst %,-Xlinker -rpath-link=%,$(XLINK_LIBDIR))

OPT = -O0
DEBUG = -g
WARN= -Wall
PTHREAD= -pthread
	
all:
	$(CC) $(OPT) $(DEBUG) $(WARN) $(LIBDIR) $(PTHREAD) $(INCDIR) $(XLINKDIR) $(LIB) $(SRC) -o $(TARGET)

clean:
	rm -rf $(TARGET)



