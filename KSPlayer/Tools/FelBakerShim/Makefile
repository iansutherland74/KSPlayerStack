CXX ?= clang++
BUILD_DIR ?= build
DYLIB_NAME ?= libKSPlayerFelBakerShim.dylib
ARCH_FLAGS ?= -arch arm64 -arch x86_64
MACOSX_DEPLOYMENT_TARGET ?= 12.0

HEADER_DIR = ../../Documents
STUB_SOURCE = src/KSPlayerFelBakerShim.cpp
REAL_BRIDGE_SOURCE = src/KSPlayerFelBakerRealCompositor.cpp
DIRECT_CORE_SOURCE = src/KSPlayerFelBakerDirectCore.cpp
SOURCES ?= $(STUB_SOURCE)
SMOKE_SOURCE = tests/smoke_load.cpp
SMOKE_BINARY = $(BUILD_DIR)/smoke_load
DYLIB = $(BUILD_DIR)/$(DYLIB_NAME)

FELBAKER_ROOT ?= external/felbaker
FELBAKER_SRC_DIR ?= $(FELBAKER_ROOT)/src
FELBAKER_BUILD_DIR ?= $(FELBAKER_ROOT)/build
FELBAKER_RPU_SOURCE ?= $(FELBAKER_SRC_DIR)/RpuProcessor.cpp
LIBDOVI_INCLUDE ?= $(FELBAKER_BUILD_DIR)/libdovi
LIBDOVI_LIB_DIR ?= $(FELBAKER_BUILD_DIR)/libdovi
LIBDOVI_LDFLAGS ?= -L"$(LIBDOVI_LIB_DIR)" -ldovi
PRIVATE_SOURCES ?=

CPPFLAGS ?=
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -fvisibility=hidden -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET)
LDFLAGS ?=
FRAMEWORKS = -framework CoreMedia -framework CoreVideo
PRIVATE_CXXFLAGS ?=
PRIVATE_LDFLAGS ?=

.PHONY: all stub real real-direct real-custom smoke clean

all: stub

stub: $(DYLIB)

$(DYLIB): $(SOURCES) $(HEADER_DIR)/KSPlayerFelBakerShim.h
	mkdir -p "$(BUILD_DIR)"
	$(CXX) $(ARCH_FLAGS) $(CPPFLAGS) $(CXXFLAGS) $(PRIVATE_CXXFLAGS) -I"$(HEADER_DIR)" \
		-dynamiclib $(SOURCES) -o "$(DYLIB)" \
		-Wl,-install_name,@rpath/$(DYLIB_NAME) $(LDFLAGS) $(PRIVATE_LDFLAGS) $(FRAMEWORKS)

real: real-direct

real-direct: clean
	$(MAKE) stub \
		SOURCES="$(STUB_SOURCE) $(REAL_BRIDGE_SOURCE) $(DIRECT_CORE_SOURCE) $(FELBAKER_RPU_SOURCE)" \
		PRIVATE_CXXFLAGS="-DKSPLAYER_FELBAKER_ENABLE_REAL_COMPOSITOR=1 -DKSPLAYER_FELBAKER_HAS_CORE_ADAPTER=1 -I\"src\" -I\"$(FELBAKER_SRC_DIR)\" -I\"$(LIBDOVI_INCLUDE)\" $(PRIVATE_CXXFLAGS)" \
		PRIVATE_LDFLAGS="$(PRIVATE_LDFLAGS) $(LIBDOVI_LDFLAGS)"

real-custom: clean
	@test -n "$(PRIVATE_SOURCES)" || (echo "real-custom requires PRIVATE_SOURCES with a core adapter implementation" && exit 2)
	$(MAKE) stub \
		SOURCES="$(STUB_SOURCE) $(REAL_BRIDGE_SOURCE) $(PRIVATE_SOURCES)" \
		PRIVATE_CXXFLAGS="-DKSPLAYER_FELBAKER_ENABLE_REAL_COMPOSITOR=1 -DKSPLAYER_FELBAKER_HAS_CORE_ADAPTER=1 -I\"src\" $(PRIVATE_CXXFLAGS)" \
		PRIVATE_LDFLAGS="$(PRIVATE_LDFLAGS)"

smoke: stub $(SMOKE_BINARY)
	"$(SMOKE_BINARY)" "$(DYLIB)"

$(SMOKE_BINARY): $(SMOKE_SOURCE) $(HEADER_DIR)/KSPlayerFelBakerShim.h
	mkdir -p "$(BUILD_DIR)"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I"$(HEADER_DIR)" "$(SMOKE_SOURCE)" -o "$(SMOKE_BINARY)" $(FRAMEWORKS)

clean:
	rm -rf "$(BUILD_DIR)"
