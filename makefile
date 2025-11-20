BUILD_DIR := .auto-build
FRACTALGEN := $(BUILD_DIR)/code/src/fractalgen/fractalgen.exe

.DEFAULT_GOAL := help

.PHONY: help generate build demo readme background

help:
	@echo "Available targets:"
	@echo "  generate    - Configure CMake build directory"
	@echo "  build       - Build the project with CMake"
	@echo "  demo        - Run basic fractalgen"
	@echo "  readme      - Render images for README"
	@echo "  clean       - Remove build directory"
	@echo "  backgrounds - Generate desktop backgrounds"
	@echo ""
	@echo "Usage: make <target>"

img:
	@mkdir img

img/demo: | img
	@mkdir img/demo

img/mandelbrot: | img
	@mkdir img/mandelbrot

img/powertower: | img
	@mkdir img/powertower

img/newton: | img
	@mkdir img/newton

img/backgrounds: | img
	@mkdir img/backgrounds

generate:
	@cmake -B $(BUILD_DIR) -S . -D CMAKE_BUILD_TYPE=RELEASE -G "MinGW Makefiles"

build:
	@cmake --build $(BUILD_DIR) --config Release

demo: build | img/demo
	$(FRACTALGEN) mandelbrot --name img/demo/mandelbrot.png
	$(FRACTALGEN) powertower --name img/demo/powertower.png
	$(FRACTALGEN) newton --name img/demo/newton.png --root -3 1 255 0 0 --root -3 -1 0 255 0 --root 1 0 0 0 255

readme: build | img/mandelbrot img/powertower img/newton
	$(FRACTALGEN) mandelbrot --name fractal.png --width 1000 --bounds -4 -1.5 1.33 1.5 --phi 0

backgrounds: build | img/backgrounds
	$(FRACTALGEN) mandelbrot --name img/backgrounds/mandelbrot-black.png --width 1920 --bounds -4 -1.5 1.33 1.5 --phi 0