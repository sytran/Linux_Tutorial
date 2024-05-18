ARM_PREFIX= arm-linux-gnueabihf-
#CC = $(ARM_PREFIX)gcc-linaro
CC = $(ARM_PREFIX)gcc-7.5.0
SRC += Basic_pThread.c

TARGET = Basic_pThread
LIBRARY += gtk-3
LIBRARY += gdk-3
LIBRARY += atk-1.0
LIBRARY += gio-2.0
LIBRARY += pangocairo-1.0
LIBRARY += gdk_pixbuf-2.0
LIBRARY += cairo-gobject
LIBRARY += pango-1.0
LIBRARY += cairo
LIBRARY += gobject-2.0
LIBRARY += glib-2.0

LIBRARYDIR +=  /opt/cross-compile/debian_libs/lib/arm-linux-gnueabihf
LIBRARYDIR += /opt/cross-compile/debian_libs/usr/lib/arm-linux-gnueabihf
LIBRARYDIR += /opt/cross-compile/debian_libs/lib
LIBRARYDIR += /opt/cross-compile/debian_libs/usr/lib
XLINK_LIBDIR += /opt/cross-compile/debian_libs/lib/arm-linux-gnueabihf
XLINK_LIBDIR += /opt/cross-compile/debian_libs/usr/lib/arm-linux-gnueabihf
INCLUDEDIR += /opt/cross-compile/debian_libs/usr/include/gtk-3.0
INCLUDEDIR += /opt/cross-compile/debian_libs/usr/include/pango-1.0
INCLUDEDIR += /opt/cross-compile/debian_libs/usr/include/gio-unix-2.0
INCLUDEDIR += /opt/cross-compile/debian_libs/usr/include/atk-1.0
INCLUDEDIR += /opt/cross-compile/debian_libs/usr/include/cairo
INCLUDEDIR += /opt/cross-compile/debian_libs/usr/include/gdk-pixbuf-2.0
INCLUDEDIR += /opt/cross-compile/debian_libs/usr/include/freetype2
INCLUDEDIR += /opt/cross-compile/debian_libs/usr/include/glib-2.0
INCLUDEDIR += /opt/cross-compile/debian_libs/usr/lib/arm-linux-gnueabihf/glib-2.0/include
INCLUDEDIR += /opt/cross-compile/debian_libs/usr/include/pixman-1
INCLUDEDIR += /opt/cross-compile/debian_libs/usr/include/libpng12
	
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



