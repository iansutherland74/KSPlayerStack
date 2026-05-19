#include "KSPlayerFelBakerShim.h"

#include <CoreMedia/CoreMedia.h>
#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>

namespace {

template <typename Function>
Function loadSymbol(void *handle, const char *name) {
    void *symbol = dlsym(handle, name);
    if (symbol == nullptr) {
        std::fprintf(stderr, "missing symbol %s: %s\n", name, dlerror());
        std::exit(2);
    }
    return reinterpret_cast<Function>(symbol);
}

} // namespace

int main(int argc, char **argv) {
    if (argc != 2) {
        std::fprintf(stderr, "usage: %s /path/to/libKSPlayerFelBakerShim.dylib\n", argv[0]);
        return 2;
    }

    void *handle = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
    if (handle == nullptr) {
        std::fprintf(stderr, "dlopen failed: %s\n", dlerror());
        return 1;
    }

    using VersionFunction = uint32_t (*)();
    using NameFunction = const char *(*)();
    using CapabilitiesFunction = uint64_t (*)();
    using ComposeFunction = int32_t (*)(
        CVPixelBufferRef,
        CVPixelBufferRef,
        const uint8_t *,
        size_t,
        CMTime,
        CVPixelBufferRef,
        char *,
        size_t
    );

    auto version = loadSymbol<VersionFunction>(handle, "ksplayer_felbaker_backend_abi_version");
    auto name = loadSymbol<NameFunction>(handle, "ksplayer_felbaker_backend_name");
    auto capabilities = loadSymbol<CapabilitiesFunction>(handle, "ksplayer_felbaker_backend_capabilities");
    auto compose = loadSymbol<ComposeFunction>(handle, "ksplayer_felbaker_compose");

    const uint32_t abiVersion = version();
    const uint64_t caps = capabilities();
    char diagnostic[512] = {};
    const int32_t status = compose(
        nullptr,
        nullptr,
        nullptr,
        0,
        CMTimeMake(0, 1),
        nullptr,
        diagnostic,
        sizeof(diagnostic)
    );

    std::printf("name: %s\n", name() == nullptr ? "(null)" : name());
    std::printf("abi: %u\n", abiVersion);
    std::printf("capabilities: 0x%llx\n", static_cast<unsigned long long>(caps));
    std::printf("compose status: %d\n", status);
    std::printf("diagnostic: %s\n", diagnostic);

    dlclose(handle);

    if (abiVersion != KSPLAYER_FELBAKER_SHIM_ABI_VERSION) {
        std::fprintf(stderr, "unexpected ABI version\n");
        return 1;
    }
    if ((caps & KSPLAYER_FELBAKER_CAPABILITY_FULL_FEL_COMPOSITION) != 0) {
        std::fprintf(stderr, "stub smoke test must not report full FEL composition\n");
        return 1;
    }
    if (status != KSPLAYER_FELBAKER_STATUS_NOT_IMPLEMENTED) {
        std::fprintf(stderr, "stub compose returned unexpected status\n");
        return 1;
    }
    return 0;
}
