CC = gcc
SRC += ping_pong_pThread_withTimer.c
TARGET = ping_pong_pThread_withTimer

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



