TARGET=bmp.so
OBJS=bmp.o
CFLAGS=-fPIC -g
LDFLAGS=-lc
%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
$(TARGET): $(OBJS)
	$(CC) -g -shared -Wl -o $@ $^ $(LDFLAGS)
clean:
	rm -rf *.o
	rm -rf bmp.so
