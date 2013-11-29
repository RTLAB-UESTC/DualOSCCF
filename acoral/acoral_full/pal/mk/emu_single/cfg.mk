PAL_LINKFLAGS  += -dynamic-linker /lib/ld-linux.so.2
PAL_EXT_FILES  += /usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o
PAL_CLIBS += -L$(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) -lc -lpthread

