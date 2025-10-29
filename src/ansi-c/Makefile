TARGET_LIB=libansic.a

C_OBJS = \
	memmove.o \
	memcmp.o \
	memcpy.o \
	memset.o \
	strcpy.o \
	strlen.o \
	strchr.o \
	strcmp.o \
	strstr.o \
	strspn.o \
	strtok.o \
	memfind.o \
	strhex32.o \
	strsafe.o \
	strbeginswith.o \
	strreplaceall.o \
	strcasecmp.o \
	lowerstring.o \
	setlongjmp.o

OBJS = $(C_OBJS)

all: $(TARGET_LIB)

PSPSDK=$(shell psp-config --pspsdk-path)

CC=psp-gcc
INCDIR = $(ARKROOT)/common/include
CFLAGS = -std=c99 -Wall -Os -G0 -fno-pic
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
