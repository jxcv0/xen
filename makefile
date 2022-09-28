CFILES = main.c src/resources.c
GCC_FLAGS = -ggdb -O3 -Wall -Wextra -Werror
GLAD_SRC = lib/glad/src/glad.c
GLAD_INCLUDE_DIR = lib/glad/include/glad
KHR_INCLUDE_DIR = lib/glad/include/KHR
STB_DIR = lib/stb
LIBS = -lm -ldl -lc -lglfw -lassimp

build:
	gcc -DL1DCLS=64 -o bin/xen \
		-I include -I ${GLAD_INCLUDE_DIR} -I ${KHR_INCLUDE_DIR} -I ${STB_DIR} \
		${GLAD_SRC} ${CFILES} ${GCC_FLAGS} ${LIBS}

