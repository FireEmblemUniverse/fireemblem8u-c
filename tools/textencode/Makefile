CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11 -O2 -s

.PHONY: clean

SRCS = textencode.c inputfile.c

textencode: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

clean:
	$(RM) textencode textencode.exe
