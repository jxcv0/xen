#!/bin/bash
gcc kv.c glad.c -I include/ -o run -ggdb -lglfw -lpthread
