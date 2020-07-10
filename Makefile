OSFLAG :=
ifeq ($(OS), Windows_NT)
	test-executable = start test.exe
else
	test-executable = xdg-open ./test
endif

.PHONY: test-compile test test-all
test-compile:
ifeq (,$(wildcard ./test/test_main.o))
	$(info Compiling test_main.cpp, this only needs to be done once.)
	@g++ ./test/test_main.cpp -c -o ./test/test_main.o
	@g++ -std=c++17 ./test/test_main.o ./test/test_tui.cpp -o ./test/test
else
	@g++ -std=c++17 ./test/test_main.o ./test/test_tui.cpp -o ./test/test
endif
test:
	@cd test && $(test-executable) --out test_output.txt
test-all: test-compile test