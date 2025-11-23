#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

namespace vroom {

struct PackageFileEntry; // Forward declaration

/**
 * @brief Interface for reading asset data from various sources (Disk, Archive).
 */
class AssetProvider {
public:
    virtual ~AssetProvider() = default;
    
    // Returns true if the asset exists in this provider
    [[nodiscard]] virtual bool exists(const std::filesystem::path& relativePath) const = 0;
    
    // Returns the raw data of the asset
    [[nodiscard]] virtual std::vector<char> readFile(const std::filesystem::path& relativePath) const = 0;
};

/**
 * @brief Provider that reads assets from a directory on disk.
 */
class DiskAssetProvider : public AssetProvider {
public:
    explicit DiskAssetProvider(const std::filesystem::path& rootPath);
    
    [[nodiscard]] bool exists(const std::filesystem::path& relativePath) const override;
    [[nodiscard]] std::vector<char> readFile(const std::filesystem::path& relativePath) const override;

private:
    std::filesystem::path m_rootPath;
};

/**
 * @brief Provider that reads assets from a package file (single file container).
 * @note Implementation details would depend on the package format.
 */
class PackageAssetProvider : public AssetProvider {
public:
    explicit PackageAssetProvider(const std::filesystem::path& packagePath);
    
    [[nodiscard]] bool exists(const std::filesystem::path& relativePath) const override;
    [[nodiscard]] std::vector<char> readFile(const std::filesystem::path& relativePath) const override;

private:
    std::filesystem::path m_packagePath;
    struct FileInfo {
        uint64_t offset;
        uint64_t size;
    };
    std::unordered_map<std::string, FileInfo> m_fileTable;
};

} // namespace vroom
