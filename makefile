run: kv.c glad.c
	gcc kv.c glad.c -I include/ -Wall -Werror -o lib/run -O3 -ggdb -ldl -lm -lglfw -lpthread -lassimp
