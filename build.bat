@echo off
gcc game/cube.c src\xen.c src\glad.c -I include\ -Wall -Werror -o bin\cube -ggdb -lpthread -lassimp lib\glfw3.dll
