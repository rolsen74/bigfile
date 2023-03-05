
###########################################################################

.PHONY: all clean revision install

###########################################################################

CC			:= gcc
STRIP		:= strip

###########################################################################

CFLAGS		:= -O2
CFLAGS		+= -W
CFLAGS		+= -Wall
CFLAGS		+= -Wwrite-strings

LDFLAGS		:= 

LIBS		:= -L.

SRCS		:= main.c

###########################################################################

ifeq ($(OS),Windows_NT)
	detected_os := Windows
else
	detected_os := $(shell uname)
endif

# --

ifeq ($(detected_os),Windows)

RM			:= del
TARGET		:= BigFile.exe

endif

# --

ifeq ($(detected_os),AmigaOS4)

RM			:= delete
TARGET		:= BigFile

endif

# --

ifeq ($(detected_os),Linux)

RM			:= rm
TARGET		:= BigFile

endif

###########################################################################

all:		$(TARGET)

OBJS		:= $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET).debug $(OBJS) $(LIBS) -Wl,--cref,-M,-Map=$@.map
	$(STRIP) --strip-all -R.comment $(TARGET).debug -o $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).debug $(TARGET).map $(OBJS)

###########################################################################

main.o:			Makefile

###########################################################################
