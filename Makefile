compile_commands:
	cmake -G Ninja -B build

build: compile_commands
	cmake --build build

test: build
ifdef T
	cd build && ctest --output-on-failure -R $(T)
else
	cd build && ctest --output-on-failure
endif

.PHONY: compile_commands build test
