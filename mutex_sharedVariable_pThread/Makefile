CC = gcc
SRC += mutex_sharedVariable_pThread.c
TARGET = mutex_sharedVariable_pThread

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
REALTIME_LIB = -lrt
	
all:
	$(CC) $(OPT) $(DEBUG) $(WARN) $(LIBDIR) $(PTHREAD) $(INCDIR) $(XLINKDIR) $(LIB) $(SRC) -o $(TARGET) $(REALTIME_LIB)

clean:
	rm -rf $(TARGET)



