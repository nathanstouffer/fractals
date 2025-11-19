BUILD_DIR := .auto-build
FRACTALGEN := $(BUILD_DIR)/code/src/fractalgen/fractalgen.exe

.PHONY: generate build demo

generate:
	cmake -B $(BUILD_DIR) -S . -D CMAKE_BUILD_TYPE=RELEASE -G "MinGW Makefiles"

build:
	cmake --build $(BUILD_DIR) --config CMAKE_BUILD_TYPE=RELEASE

demo: build
	$(FRACTALGEN) mandelbrot
