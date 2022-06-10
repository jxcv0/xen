@echo off
gcc tests\lm_tests.c -I include\ -o bin\lm_tests -ggdb
gcc tests\model_tests.c src\glad.c -I include\ -o bin\model_tests -ggdb lib\glfw3.dll -lassimp

.\bin\lm_tests.exe
.\bin\model_tests.exe
