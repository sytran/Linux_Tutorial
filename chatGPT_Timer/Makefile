CC = gcc
SRC += chatGPT_Timers.c
TARGET = chatGPT_Timers

LIBRARY += 
LIBRARYDIR +=  
XLINK_LIBDIR += 
INCLUDEDIR += /user/include

INCDIR = $(patsubst %,-I%,$(INCLUDEDIR))
LIBDIR = $(patsubst %,-L%,$(LIBRARYDIR))
LIB = $(patsubst %,-l%,$(LIBRARY))
XLINKDIR = $(patsubst %,-Xlinker -rpath-link=%,$(XLINK_LIBDIR))

#define	__USE_POSIX
#define	__USE_POSIX199309

#OPT = -O0 -D__USE_POSIX -D__USE_POSIX199309
OPT = -O0 
DEBUG = -g
WARN= -Wall
PTHREAD= -pthread
REALTIME_LIB = -lrt

	
all:
	$(CC) $(OPT) $(DEBUG) $(WARN) $(LIBDIR) $(PTHREAD) $(INCDIR) $(XLINKDIR) $(LIB) $(SRC) -o $(TARGET) $(REALTIME_LIB)

clean:
	rm -rf $(TARGET)



