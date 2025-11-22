BUILD_DIR := .auto-build
FRACTALGEN := $(BUILD_DIR)/code/src/fractalgen/fractalgen.exe

.DEFAULT_GOAL := help

.PHONY: help generate build demo readme backgrounds mandelbrot-backgrounds powertower-backgrounds newton-backgrounds

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

BCKGRND_WDTH := 1000

mandelbrot-backgrounds: build | img/backgrounds
	$(FRACTALGEN) mandelbrot --name img/backgrounds/mandelbrot-black.png --width $(BCKGRND_WDTH) --bounds -4 -1.5 1.33 1.5 --phi 0 --color 0 0 0 --diverging 0 0 0

powertower-backgrounds: build | img/backgrounds
	$(FRACTALGEN) powertower --name img/backgrounds/powertower-black-and-white.png --width $(BCKGRND_WDTH) --bounds -4 -1.5 1.33 1.5 --phi 0 --color 0 0 0 --diverging 255 255 255
	$(FRACTALGEN) powertower --name img/backgrounds/powertower-black-and-yellow.png --width $(BCKGRND_WDTH) --bounds -4 -1.5 1.33 1.5 --phi 0 --color 0 0 0 --diverging 255 255 0

# TODO (stouff) investigate newton fractals for a bug -- the colors seem to be mismatched
newton-backgrounds: build | img/backgrounds
# 	$(FRACTALGEN) newton --name img/backgrounds/newton-blue.png --width $(BCKGRND_WDTH) --bounds -20 -11.25 20 11.25 --phi 0 --root -1 0 0 10 170 --root 0 1 0 10 70 --root 1 0 0 5 30 --root 0 -1 36 70 255
# 	$(FRACTALGEN) newton --name img/backgrounds/newton-green.png --width $(BCKGRND_WDTH) --bounds -20 -11.25 20 11.25 --phi 0 --root -7 0 135 255 167 --root -4.25 0 29 130 56 --root 4.25 0 0 79 21 --root 7 0 0 0 0
	$(FRACTALGEN) newton --name img/backgrounds/newton-pcp.png --width $(BCKGRND_WDTH) --bounds -20 -11.25 20 11.25 --phi 0 --root 5 -4.2 72 16 94 --root 5 4.2 255 167 129 --root 15 0 89 255 255

backgrounds: mandelbrot-backgrounds powertower-backgrounds newton-backgrounds