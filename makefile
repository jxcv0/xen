mkbin:
	@if [ ! -d "bin" ]; then mkdir bin; fi

mktests: mkbin
	@if [ ! -d "bin/tests" ]; then mkdir bin/tests; fi

build_tests: mktests tests/lm_tests.c tests/model_tests.c src/glad.c
	@gcc tests/lm_tests.c src/glad.c -I include/ -o bin/tests/lm_tests -ggdb -lm
	@gcc tests/model_tests.c src/glad.c -I include/ -o bin/tests/model_tests -ggdb -lm -lglfw -lassimp

run_tests: build_tests
	@./bin/tests/lm_tests
	@./bin/tests/model_tests

debug: mkbin src/xen.c src/glad.c
	@gcc src/xen.c src/glad.c -I include/ -Wall -Werror -o bin/xen -ggdb -ldl -lm -lglfw -lpthread -lassimp
	@./bin/xen

fast: mkbin src/xen.c src/glad.c
	@gcc src/xen.c src/glad.c -I include/ -o bin/xen -O3 -ldl -lm -lglfw -lpthread -lassimp
	@./bin/xen

clean:
	@rm bin/*
