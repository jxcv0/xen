CFILES = main.c src/resources.c
GCCFLAGS = -ggdb -O3 -Wall -Wextra -Werror
LIBS = -lm -lc -lglfw

build:
	gcc -DL1DCLS=64 -o bin/xen -I include -I src -I opengl $(CFILES) $(GCCFLAGS) $(LIBS) $(VARS)


