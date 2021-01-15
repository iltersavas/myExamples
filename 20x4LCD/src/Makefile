# file: Makefile
# name: ilter SAVAS
# date: 2020-Aust

# compiler
CXX	= arm-linux-gnueabihf-gcc


# .o files
OFILES = simplegpio.o \
	   	 driver.o \
	   	 main.o

# target name
TARGET	= characterLCD

INCLUDES = -MD \
		   -I../inc/  
		   
#LIBS
LIBS =  

SRCS = simplegpio.c \
	   driver.c \
	   main.c

# compile
all:	#$(OFILES)
	$(CXX) $(INCLUDES) $(EXTRA_CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)
	-@uptime -s	
	-@date
	-@ls $(TARGET)

# clean up
clean:	
	/bin/rm -f *.o *~
	/bin/rm -f $(TARGET)
	/bin/rm -f $(TARGET).*

