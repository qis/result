# usage: make run|test|benchmark|install|package|format|clean|ports
#
#  run        - build and execute program in debug mode
#  test       - build and execute tests in debug and release mode
#  benchmark  - build and execute benchmarks in release mode
#  install    - build and install program in release mode
#  package    - create a package for distribution
#  format     - use clang-format on all sources
#  clean      - remove curent build directory
#  ports      - install vcpkg dependencies
#
MAKEFLAGS += --no-print-directory

TARGET	:= $(shell cmake -P res/scripts/target.cmake 2>&1)
SOURCE	:= $(shell cmake -P res/scripts/source.cmake 2>&1)

ifeq ($(OS),Windows_NT)
SCRIPT	:= $(dir $(shell where vcpkg.exe))scripts
SYSTEM	:= windows
LINKER	:= msvc
else
SCRIPT	:= $(dir $(shell which vcpkg))scripts
SYSTEM	:= linux
LINKER	:= llvm
endif

CONFIG	:= -DCMAKE_TOOLCHAIN_FILE:PATH="$(SCRIPT)/buildsystems/vcpkg.cmake"
CONFIG	+= -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE:PATH="$(SCRIPT)/toolchains/$(SYSTEM).cmake"
CONFIG	+= -DVCPKG_TARGET_TRIPLET=$(VCPKG_DEFAULT_TRIPLET)

all: build/$(LINKER)/debug/CMakeCache.txt
	@cmake --build build/$(LINKER)/debug --target $(TARGET)

run: build/$(LINKER)/debug/CMakeCache.txt
	@cmake --build build/$(LINKER)/debug --target $(TARGET)
	@build/$(LINKER)/debug/$(TARGET)

test: build/$(LINKER)/debug/CMakeCache.txt build/$(LINKER)/release/CMakeCache.txt
	@cmake --build build/$(LINKER)/debug
	@cmake --build build/$(LINKER)/release
	@cmake --build build/$(LINKER)/debug --target test
	@cmake --build build/$(LINKER)/release --target test

benchmark: build/$(LINKER)/release/CMakeCache.txt
	@cmake --build build/$(LINKER)/release --target $(TARGET)
	@build/$(LINKER)/release/$(TARGET) --benchmark

install: build/$(LINKER)/release/CMakeCache.txt
	@cmake --build build/$(LINKER)/release --target install

package: build/$(LINKER)/release/CMakeCache.txt
	@cmake --build build/$(LINKER)/release --target package

format:
	@clang-format -i $(SOURCE)

clean:
	@cmake -E remove_directory build/$(LINKER)

ifeq ($(OS),Windows_NT)
ports: ports.txt
	@vcpkg install tbb @$<
else
ports: ports.txt
	@vcpkg install @$<
endif

build/$(LINKER)/debug/CMakeCache.txt: CMakeLists.txt
	@cmake -GNinja $(CONFIG) -DCMAKE_BUILD_TYPE=Debug \
	  -DCMAKE_INSTALL_PREFIX="$(CURDIR)/install/$(LINKER)/debug" \
	  -B build/$(LINKER)/debug

build/$(LINKER)/release/CMakeCache.txt: CMakeLists.txt
	@cmake -GNinja $(CONFIG) -DCMAKE_BUILD_TYPE=Release \
	  -DCMAKE_INSTALL_PREFIX="$(CURDIR)/install/$(LINKER)/release" \
	  -B build/$(LINKER)/release

.PHONY: all run test benchmark install package format clean ports
