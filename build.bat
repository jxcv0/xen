@echo off
gcc kv.c glad.c -I include/ -Wall -Werror -o bin\run -O3 -ggdb -lpthread -lassimp lib\glfw3.dll
