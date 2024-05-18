ARM_PREFIX= arm-linux-gnueabihf-
#CC = $(ARM_PREFIX)gcc-linaro
CC = gcc
SRC += StateMc_pThread.c

TARGET = StateMc_pThread
LIBRARY += 

LIBRARYDIR +=  
XLINK_LIBDIR += 
INCLUDEDIR += 

	
OPT = -O0
DEBUG = -g
WARN= -Wall
PTHREAD= -pthread
	
INCDIR = $(patsubst %,-I%,$(INCLUDEDIR))
LIBDIR = $(patsubst %,-L%,$(LIBRARYDIR))
LIB = $(patsubst %,-l%,$(LIBRARY))
XLINKDIR = $(patsubst %,-Xlinker -rpath-link=%,$(XLINK_LIBDIR))
all:
	$(CC) $(OPT) $(DEBUG) $(WARN) $(LIBDIR) $(PTHREAD) $(INCDIR) $(XLINKDIR) $(LIB) $(SRC) -o $(TARGET)

clean:
	rm -rf $(TARGET)



