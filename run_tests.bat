@echo off
gcc tests\lm_tests.c -I include\ -o bin\lm_tests -ggdb
gcc tests\model_tests.c -I include\ -o bin\model_tests -ggdb

.\bin\lm_tests.exe
.\bin\model_tests.exe

