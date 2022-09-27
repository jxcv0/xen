CFILES = main.c src/resources.c
GCCFLAGS = -ggdb -O3 -Wall -Wextra -Werror
LIBS = -lm -lc -lglfw
VARS = -DL1DCLS=$(getconf LEVEL1_DCACHE_LINESIZE)

build:
	gcc -o bin/xen -I include -I src -I opengl $(CFILES) $(GCCFLAGS) $(LIBS) $(VARS)
