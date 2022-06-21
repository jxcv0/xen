xen_source_files = src/xen.c

mkbin:
	@if [ ! -d "bin" ]; then mkdir bin; fi

mktests: mkbin
	@if [ ! -d "bin/tests" ]; then mkdir bin/tests; fi

build_tests: mktests tests/lm_tests.c src/xen.c src/glad.c
	@gcc tests/lm_tests.c src/xen.c src/glad.c -I include/ -o bin/tests/lm_tests -ggdb -ldl -lm -lglfw -lpthread -lassimp

run_tests: build_tests
	@./bin/tests/lm_tests

cube: mkbin game/cube.c src/xen.c src/glad.c
	@gcc game/cube.c src/xen.c src/glad.c -I include/ -Wall -Werror -o bin/cube -ggdb -ldl -lm -lglfw -lpthread -lassimp

run_cube:
	@./bin/cube

clean:
	@rm bin/*
