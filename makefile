BUILD_DIR := .auto-build
FRACTALGEN := $(BUILD_DIR)/code/src/fractalgen/fractalgen.exe

.PHONY: generate build demo readme

generate:
	@cmake -B $(BUILD_DIR) -S . -D CMAKE_BUILD_TYPE=RELEASE -G "MinGW Makefiles"

build:
	@cmake --build $(BUILD_DIR) --config CMAKE_BUILD_TYPE=RELEASE

demo: | build
	@$(FRACTALGEN) mandelbrot

img:
	@mkdir img

img/mandelbrot: | img
	@mkdir img/mandelbrot

img/powertower: | img
	@mkdir imgpowertower

img/newton: | img
	@mkdir img/newton

readme: | build
	@$(FRACTALGEN) mandelbrot --name img/mandelbrot/tmp --width 1000 --bounds -4 -1.5 1.33 1.5 --rho 0
