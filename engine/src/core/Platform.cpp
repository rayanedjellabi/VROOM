#include "vroom/core/Platform.hpp"

#if defined(_WIN32)
    #include <windows.h>
    #include <libloaderapi.h>
#elif defined(__linux__)
    #include <unistd.h>
    #include <limits.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    #include <limits.h>
#endif

namespace vroom {

std::filesystem::path Platform::getExecutablePath() {
#if defined(_WIN32)
    char buffer[MAX_PATH];
    if (GetModuleFileNameA(NULL, buffer, MAX_PATH) != 0) {
        return std::filesystem::path(buffer);
    }
#elif defined(__linux__)
    char buffer[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
    if (count != -1) {
        return std::filesystem::path(std::string(buffer, (count > 0) ? count : 0));
    }
#elif defined(__APPLE__)
    char buffer[PATH_MAX];
    uint32_t bufsize = PATH_MAX;
    if (_NSGetExecutablePath(buffer, &bufsize) == 0) {
        // Resolving symlinks
        char realPath[PATH_MAX];
        if (realpath(buffer, realPath)) {
             return std::filesystem::path(realPath);
        }
        return std::filesystem::path(buffer);
    }
#endif
    return std::filesystem::current_path(); // Fallback
}

std::filesystem::path Platform::getExecutableDir() {
    return getExecutablePath().parent_path();
}

} // namespace vroom

