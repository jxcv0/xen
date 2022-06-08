lm_tests: tests/lm_tests.c
	gcc tests/lm_tests.c -I include/ -o bin/lm_tests -ggdb -lm

debug: src/xen.c src/glad.c
	gcc src/xen.c src/glad.c -I include/ -Wall -Werror -o bin/xen -ggdb -ldl -lm -lglfw -lpthread -lassimp

fast: src/xen.c src/glad.c
	gcc src/xen.c src/glad.c -I include/ -Wall -Werror -o -03 bin/xen -ldl -lm -lglfw -lpthread -lassimp
