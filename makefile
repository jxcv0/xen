build_tests: tests/lm_tests.c tests/model_tests.c src/glad.c
	gcc tests/lm_tests.c src/glad.c -I include/ -o bin/lm_tests -ggdb -lm
	gcc tests/model_tests.c src/glad.c -I include/ -o bin/model_tests -ggdb -lm -lglfw -lassimp

run_tests:
	./bin/lm_tests
	./bin/model_tests

debug: src/xen.c src/glad.c
	gcc src/xen.c src/glad.c -I include/ -Wall -Werror -o bin/xen -ggdb -ldl -lm -lglfw -lpthread -lassimp
	./bin/xen

fast: src/xen.c src/glad.c
	gcc src/xen.c src/glad.c -I include/ -Wall -Werror -o bin/xen -O3 -ldl -lm -lglfw -lpthread -lassimp
	./bin/xen

clean:
	rm bin/*
