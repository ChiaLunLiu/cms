
LIB_CMC2=libcmc2.so
LIB_SIMCLIST=libsimclist.so
SRCS    := $(wildcard *.c) $(wildcard lib/*.c)
OBJS    := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

CFLAGS  += -fno-builtin -O2 -fPIC -shared  -I../include 

all:
	$(CC) $(CFLAGS) cmc.c -o libcmc.so -lminimsg
	$(CC) $(CFLAGS) simclist.c -o libsimclist.so


clean:
	rm libsimclist.so libcmc.so	
#library: $(OBJS)
#	$(CC) -shared -o $(OBJDIR)/../lib/$(LIB_CMC2) $(OBJDIR)/cmc2.o 
#	$(CC) -shared -o $(OBJDIR)/../lib/$(LIB_SIMCLIST) $(OBJDIR)/simclist.o 
#program: 

#$(OBJDIR)/%.o: cmc2.c
#	$(CC) $(CFLAGS) -c cmc2.c -o $(OBJDIR)/cmc2.o

#clean:
#	$(RM) -f $(OBJDIR)/../lib/$(LIB_CMC2) $(OBJDIR)/../lib/$(LIB_SIMCLIST)  $(OBJDIR)/cmc2.o $(OBJDIR)/simclist.o
