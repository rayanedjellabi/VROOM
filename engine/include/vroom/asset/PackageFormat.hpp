#pragma once

#include "vroom/asset/AssetProvider.hpp"
#include "vroom/logging/LogMacros.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>

namespace vroom {

struct PackageHeader {
    char magic[4] = {'V', 'R', 'P', 'K'}; // VRoom PacKage
    uint32_t version = 1;
    uint32_t fileCount = 0;
};

struct PackageFileEntry {
    char path[256]; // Fixed size path for simplicity
    uint64_t offset;
    uint64_t size;
};

} // namespace vroom

