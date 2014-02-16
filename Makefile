sensitizer: main.c
	$(CC) -Wall -Wextra -Werror -mmacosx-version-min=10.9 -o sensitizer main.c

all: sensitizer

clean:
	rm sensitizer
