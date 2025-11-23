#pragma once

#include <string>
#include <filesystem>

namespace vroom {

class Platform {
public:
    /**
     * @brief Gets the absolute path to the executable file.
     */
    static std::filesystem::path getExecutablePath();

    /**
     * @brief Gets the directory containing the executable.
     */
    static std::filesystem::path getExecutableDir();
};

} // namespace vroom

