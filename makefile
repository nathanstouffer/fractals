BUILD_DIR := .auto-build
FRACTALGEN := $(BUILD_DIR)/code/src/fractalgen/fractalgen.exe

WIDTH = 2000

.DEFAULT_GOAL := help

.PHONY: help generate build demo readme fractals mandelbrot powertower newton

help:
	@echo "Available targets:"
	@echo "  generate    - Configure CMake build directory"
	@echo "  build       - Build the project with CMake"
	@echo "  demo        - Run basic fractalgen"
	@echo "  fractals    - Generate desktop backgrounds"
	@echo ""
	@echo "Usage: make <target>"

img:
	@mkdir img

img/demo: | img
	@mkdir img/demo

img/generated: | img
	@mkdir img/generated

generate:
	@cmake -B $(BUILD_DIR) -S . -D CMAKE_BUILD_TYPE=RELEASE -G "MinGW Makefiles"

build:
	@cmake --build $(BUILD_DIR) --config Release

demo: build | img/demo
	$(FRACTALGEN) mandelbrot --name img/demo/mandelbrot.png
	$(FRACTALGEN) powertower --name img/demo/powertower.png --bounds -5.2 -1.75 1 1.75 --color 0 0 0 --diverging 255 255 0
	$(FRACTALGEN) newton --name img/demo/newton.png --bounds -20 -11.25 20 11.25 --phi 0 --root 5 -5.7735 0 255 0 --root 5 5.7735 0 0 255 --root 15 0 255 0 0

mandelbrot: build | img/generated
	$(FRACTALGEN) mandelbrot --name img/generated/mandelbrot-black.png --width $(WIDTH) --bounds -4 -1.5 1.33 1.5 --phi 0 --color 0 0 0 --diverging 0 0 0
	$(FRACTALGEN) mandelbrot --name img/generated/mandelbrot-blue.png --width $(WIDTH) --bounds -4 -1.5 1.33 1.5 --phi 0 --color 0 0 0 --diverging 0 0 153
	$(FRACTALGEN) mandelbrot --name img/generated/mandelbrot-purple.png --width $(WIDTH) --bounds -4 -1.5 1.33 1.5 --phi 0 --color 0 0 0 --diverging 89 0 89
	$(FRACTALGEN) mandelbrot --name img/generated/mandelbrot-green-teardrop.png --width $(WIDTH) --bounds -6.66 -3 4 3 --phi 3.1415926535 --color 0 0 0 --diverging 0 102 25
	$(FRACTALGEN) mandelbrot --name img/generated/mandelbrot-black-spiral.png --width $(WIDTH) --bounds -0.798981 -0.166236 -0.798059 -0.165716 --phi 0 --color 0 0 0 --diverging 0 0 0
	$(FRACTALGEN) mandelbrot --name img/generated/mandelbrot-green-diagonal.png --width $(WIDTH) --bounds -0.833432 0.205285 -0.831393 0.206432 --phi 0 --color 0 0 0 --diverging 0 50 0
	$(FRACTALGEN) mandelbrot --name img/generated/mandelbrot-teal-spiky.png --width $(WIDTH) --bounds -0.906264 -0.268592 -0.905953 -0.268417 --phi 0 --color 0 0 0 --diverging 0 128 128

powertower: build | img/generated
	$(FRACTALGEN) powertower --name img/generated/powertower-black-and-yellow.png --width $(WIDTH) --bounds -5.2 -1.75 1 1.75 --phi 0 --color 0 0 0 --diverging 255 255 0
	$(FRACTALGEN) powertower --name img/generated/powertower-white-and-black.png --width $(WIDTH) --bounds -8.3 -3.25 3.25 3.25 --phi 0 --color 255 255 255 --diverging 0 0 0

newton: build | img/generated
	$(FRACTALGEN) newton --name img/generated/newton-blue.png --width $(WIDTH) --bounds -20 -11.25 20 11.25 --phi 0 --root -1 0 0 5 30 --root 0 1 36 70 255 --root 1 0 0 10 170 --root 0 -1 0 10 70
	$(FRACTALGEN) newton --name img/generated/newton-green.png --width $(WIDTH) --bounds -20 -11.25 20 11.25 --phi 0 --root -7 0 135 255 167 --root -4.25 0 29 130 56 --root 4.25 0 0 79 21 --root 7 0 0 0 0
	$(FRACTALGEN) newton --name img/generated/newton-rgb.png --width $(WIDTH) --bounds -20 -11.25 20 11.25 --phi 0 --root 5 -5.7735 0 255 0 --root 5 5.7735 0 0 255 --root 15 0 255 0 0
	$(FRACTALGEN) newton --name img/generated/newton-pcp.png --width $(WIDTH) --bounds -20 -11.25 20 11.25 --phi 0 --root 5 -4.25 72 16 94 --root 5 4.25 255 167 129 --root 15 0 89 255 255
	$(FRACTALGEN) newton --name img/generated/newton-purple.png --width $(WIDTH) --bounds -20 -11.25 20 11.25 --phi 0 --root 0 0 54 15 90 --root -15 0 255 215 0 --root 0 5 224 176 255 --root 15 0 255 215 0 --root 0 -5 224 176 255
	$(FRACTALGEN) newton --name img/generated/newton-brown-and-green.png --width $(WIDTH) --bounds -20 -11.25 20 11.25 --phi 0 --root -15 0 36 16 2 --root -5 -7 0 80 0 --root -5 7 90 165 90

fractals: mandelbrot powertower newton