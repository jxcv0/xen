@echo off
gcc src/xen.c src/glad.c -I include/ -Wall -Werror -o bin\run -O3 -ggdb -lpthread -lassimp lib\glfw3.dll
