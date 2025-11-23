#pragma once

#include "vroom/asset/Asset.hpp"
#include "vroom/asset/AssetProvider.hpp"
#include "vroom/asset/ShaderCompiler.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>
#include <filesystem>
#include <mutex>
#include <optional>

namespace vroom {

/**
 * @brief Manages asset loading, caching, and lifecycle.
 */
class AssetManager {
public:
    AssetManager();
    ~AssetManager();

    // Prevent copying
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    /**
     * @brief Registers a new asset provider.
     * @param provider The provider to register.
     */
    void addProvider(std::unique_ptr<AssetProvider> provider);

    /**
     * @brief Clears all registered providers.
     * Useful when switching projects or resetting the asset system.
     */
    void clearProviders();

    /**
     * @brief Sets the shader compiler instance.
     * @param compiler The shader compiler to use.
     */
    void setShaderCompiler(std::unique_ptr<ShaderCompiler> compiler);

    /**
     * @brief Gets the shader compiler instance.
     */
    ShaderCompiler* getShaderCompiler() const { return m_compiler.get(); }

    /**
     * @brief Loader function type.
     * Takes raw data and the path (for debugging/metadata), returns a shared_ptr to T.
     */
    template<typename T>
    using AssetLoader = std::function<std::shared_ptr<T>(const std::vector<char>&, const std::string&)>;

    /**
     * @brief Registers a loader for a specific asset type.
     * @tparam T The asset type.
     * @param loader The function to call to create the asset from raw data.
     */
    template<typename T>
    void registerLoader(AssetLoader<T> loader) {
        m_loaders[std::type_index(typeid(T))] = [loader](const std::vector<char>& data, const std::string& path) -> std::shared_ptr<Asset> {
            return loader(data, path);
        };
    }

    /**
     * @brief Loads an asset of type T.
     * @tparam T The type of asset to load (must inherit from Asset).
     * @param path The path to the asset relative to registered providers.
     * @return Shared pointer to the loaded asset, or nullptr if loading failed.
     */
    template<typename T>
    std::shared_ptr<T> getAsset(const std::string& path) {
        std::lock_guard<std::mutex> lock(m_assetsMutex);
        
        auto it = m_assets.find(path);
        if (it != m_assets.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }

        auto rawData = readRawAsset(path);
        if (!rawData) {
            return nullptr;
        }

        auto asset = loadFromRaw<T>(*rawData, path);
        if (asset) {
            asset->setPath(path);
            m_assets[path] = asset;
        }
        
        return asset;
    }

    /**
     * @brief Reloads an asset, bypassing the cache.
     * Note: This replaces the asset in the cache. Existing shared_ptrs will still point to the old asset.
     * Use with caution or implement an observer pattern for asset updates.
     */
    template<typename T>
    std::shared_ptr<T> reloadAsset(const std::string& path) {
        std::lock_guard<std::mutex> lock(m_assetsMutex);
        
        auto rawData = readRawAsset(path);
        if (!rawData) {
            return nullptr;
        }

        auto asset = loadFromRaw<T>(*rawData, path);
        if (asset) {
            asset->setPath(path);
            m_assets[path] = asset;
        }
        
        return asset;
    }

private:
    [[nodiscard]] std::optional<std::vector<char>> readRawAsset(const std::filesystem::path& path) const;

    template<typename T>
    std::shared_ptr<T> loadFromRaw(const std::vector<char>& data, const std::string& path) {
        auto it = m_loaders.find(std::type_index(typeid(T)));
        if (it != m_loaders.end()) {
            auto asset = it->second(data, path);
            return std::dynamic_pointer_cast<T>(asset);
        }
        return nullptr;
    }

    std::vector<std::unique_ptr<AssetProvider>> m_providers;
    std::unordered_map<std::string, std::shared_ptr<Asset>> m_assets;
    mutable std::mutex m_assetsMutex;
    std::unique_ptr<ShaderCompiler> m_compiler;

    using AnyLoader = std::function<std::shared_ptr<Asset>(const std::vector<char>&, const std::string&)>;
    std::unordered_map<std::type_index, AnyLoader> m_loaders;
};

} // namespace vroom
