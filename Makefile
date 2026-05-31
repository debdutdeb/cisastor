compile_commands:
	cmake -G Ninja -B build

build: compile_commands
	cmake --build build

test: build
	cd build && ctest

.PHONY: compile_commands build test
