# Makefile
CC := gcc
CFLAGS :=  -O1
OBJS := grpwk.o stream.o search.o visualize.o

grpwk: $(OBJS)
		$(CC) -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
