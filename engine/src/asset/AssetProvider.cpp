#include "vroom/asset/AssetProvider.hpp"
#include "vroom/asset/PackageFormat.hpp"
#include "vroom/logging/LogMacros.hpp"
#include <fstream>
#include <cstring>

namespace vroom {

DiskAssetProvider::DiskAssetProvider(const std::filesystem::path& rootPath)
    : m_rootPath(rootPath) {
}

bool DiskAssetProvider::exists(const std::filesystem::path& relativePath) const {
    return std::filesystem::exists(m_rootPath / relativePath);
}

std::vector<char> DiskAssetProvider::readFile(const std::filesystem::path& relativePath) const {
    std::filesystem::path fullPath = m_rootPath / relativePath;
    std::ifstream file(fullPath, std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        // LOG_ENGINE_ERROR("Failed to open file: " + fullPath.string());
        return {};
    }

    auto size = file.tellg();
    if (size <= 0) {
        return {};
    }

    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(static_cast<size_t>(size));
    if (file.read(buffer.data(), size)) {
        return buffer;
    }
    
    return {};
}

PackageAssetProvider::PackageAssetProvider(const std::filesystem::path& packagePath)
    : m_packagePath(packagePath) {
    
    std::ifstream file(m_packagePath, std::ios::binary);
    if (!file.is_open()) {
        LOG_ENGINE_ERROR("Failed to open package file: " + m_packagePath.string());
        return;
    }

    PackageHeader header;
    if (!file.read(reinterpret_cast<char*>(&header), sizeof(PackageHeader))) {
        LOG_ENGINE_ERROR("Failed to read package header: " + m_packagePath.string());
        return;
    }

    if (std::strncmp(header.magic, "VRPK", 4) != 0) {
        LOG_ENGINE_ERROR("Invalid package magic in file: " + m_packagePath.string());
        return;
    }

    if (header.version != 1) {
        LOG_ENGINE_ERROR("Unsupported package version: " + std::to_string(header.version));
        return;
    }

    for (uint32_t i = 0; i < header.fileCount; ++i) {
        PackageFileEntry entry;
        if (file.read(reinterpret_cast<char*>(&entry), sizeof(PackageFileEntry))) {
            m_fileTable[entry.path] = {entry.offset, entry.size};
        } else {
            LOG_ENGINE_ERROR("Failed to read file entry " + std::to_string(i));
            break;
        }
    }
}

bool PackageAssetProvider::exists(const std::filesystem::path& relativePath) const {
    return m_fileTable.find(relativePath.string()) != m_fileTable.end();
}

std::vector<char> PackageAssetProvider::readFile(const std::filesystem::path& relativePath) const {
    auto it = m_fileTable.find(relativePath.string());
    if (it == m_fileTable.end()) {
        return {};
    }

    std::ifstream file(m_packagePath, std::ios::binary);
    if (!file.is_open()) {
        return {};
    }

    file.seekg(it->second.offset, std::ios::beg);
    
    std::vector<char> buffer(it->second.size);
    if (file.read(buffer.data(), it->second.size)) {
        return buffer;
    }

    return {};
}

} // namespace vroom
