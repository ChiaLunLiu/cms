
APP	= cmscfg

SRCS    := $(wildcard *.c) $(wildcard lib/*.c)
OBJS    := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

CFLAGS  += -fno-builtin -O2 -I../include
LDFLAGS += -L../lib -lminimsg -lcmc

library:

program:
	$(CC) $(CFLAGS) cmscfg.c  -o cmscfg  $(LDFLAGS)

all: $(OBJDIR)/$(APP)

$(OBJDIR)/$(APP): $(OBJDIR)/.stamp_gen_header $(OBJS)
	$(CC) $(patsubst %.o,$(OBJDIR)/%.o,$(notdir $(OBJS))) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$(@F)

clean:
	rm -f $(OBJDIR)/cmscfg2.o
