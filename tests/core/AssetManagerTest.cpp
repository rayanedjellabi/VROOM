#include <gtest/gtest.h>
#include "vroom/asset/AssetManager.hpp"
#include "vroom/asset/AssetProvider.hpp"
#include "vroom/asset/PackageFormat.hpp"
#include "vroom/asset/ShaderAsset.hpp"
#include "vroom/asset/ShaderCompiler.hpp"
#include <fstream>
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

// Mock Asset
class TestAsset : public vroom::Asset {
public:
    std::string content;
};

// Mock Compiler
class MockShaderCompiler : public vroom::ShaderCompiler {
public:
    std::optional<std::vector<char>> compile(
        const std::filesystem::path&,
        const std::string& source,
        vroom::ShaderStage
    ) override {
        // Return "compiled" data which is just source reversed
        std::string compiled = source;
        std::reverse(compiled.begin(), compiled.end());
        return std::vector<char>(compiled.begin(), compiled.end());
    }
};

class AssetManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a temporary directory for assets
        testDir = fs::temp_directory_path() / "vroom_asset_test";
        if (fs::exists(testDir)) {
            fs::remove_all(testDir);
        }
        fs::create_directories(testDir);

        // Create a test file
        std::ofstream file(testDir / "test.txt");
        file << "Hello, Asset!";
        file.close();
        
        // Create a dummy shader file
        std::ofstream shader(testDir / "test.vert");
        shader << "void main() {}";
        shader.close();

        // Create a dummy package file
        createTestPackage(testDir / "assets.vpk");

        manager = std::make_unique<vroom::AssetManager>();
        // Add Disk provider first
        manager->addProvider(std::make_unique<vroom::DiskAssetProvider>(testDir));
    }

    void createTestPackage(const fs::path& path) {
        std::ofstream out(path, std::ios::binary);
        
        vroom::PackageHeader header;
        header.fileCount = 2;
        
        vroom::PackageFileEntry entry1;
        std::strncpy(entry1.path, "pkg_file.txt", sizeof(entry1.path));
        std::string content1 = "Hello, Package!";
        entry1.size = content1.size();
        
        vroom::PackageFileEntry entry2;
        std::strncpy(entry2.path, "subdir/data.bin", sizeof(entry2.path));
        std::string content2 = "DATA";
        entry2.size = content2.size();

        uint64_t offset = sizeof(vroom::PackageHeader) + 2 * sizeof(vroom::PackageFileEntry);
        entry1.offset = offset;
        entry2.offset = offset + entry1.size;

        out.write(reinterpret_cast<char*>(&header), sizeof(header));
        out.write(reinterpret_cast<char*>(&entry1), sizeof(entry1));
        out.write(reinterpret_cast<char*>(&entry2), sizeof(entry2));
        
        out.write(content1.c_str(), content1.size());
        out.write(content2.c_str(), content2.size());
        out.close();
    }

    void TearDown() override {
        if (fs::exists(testDir)) {
            fs::remove_all(testDir);
        }
    }

    fs::path testDir;
    std::unique_ptr<vroom::AssetManager> manager;
};

TEST_F(AssetManagerTest, LoadAsset) {
    // Register loader for TestAsset
    manager->registerLoader<TestAsset>([](const std::vector<char>& data, const std::string&) {
        auto asset = std::make_shared<TestAsset>();
        asset->content = std::string(data.begin(), data.end());
        return asset;
    });

    auto asset = manager->getAsset<TestAsset>("test.txt");
    ASSERT_NE(asset, nullptr);
    EXPECT_EQ(asset->content, "Hello, Asset!");
    EXPECT_EQ(asset->getPath(), "test.txt");
}

TEST_F(AssetManagerTest, AssetCaching) {
    manager->registerLoader<TestAsset>([](const std::vector<char>& data, const std::string&) {
        auto asset = std::make_shared<TestAsset>();
        asset->content = std::string(data.begin(), data.end());
        return asset;
    });

    auto asset1 = manager->getAsset<TestAsset>("test.txt");
    auto asset2 = manager->getAsset<TestAsset>("test.txt");

    EXPECT_EQ(asset1, asset2);
}

TEST_F(AssetManagerTest, AssetNotFound) {
     manager->registerLoader<TestAsset>([](const std::vector<char>&, const std::string&) {
        return std::make_shared<TestAsset>();
    });

    auto asset = manager->getAsset<TestAsset>("nonexistent.txt");
    EXPECT_EQ(asset, nullptr);
}

TEST_F(AssetManagerTest, ClearProviders) {
    manager->clearProviders();
    
     manager->registerLoader<TestAsset>([](const std::vector<char>&, const std::string&) {
        return std::make_shared<TestAsset>();
    });

    auto asset = manager->getAsset<TestAsset>("test.txt");
    EXPECT_EQ(asset, nullptr);
}

TEST_F(AssetManagerTest, ShaderCompilation) {
    manager->setShaderCompiler(std::make_unique<MockShaderCompiler>());
    
    manager->registerLoader<vroom::ShaderAsset>([this](const std::vector<char>& data, const std::string& path) {
        std::string ext = fs::path(path).extension().string();
        vroom::ShaderStage stage = vroom::ShaderStage::Unknown;
        if (ext == ".vert") stage = vroom::ShaderStage::Vertex;
        
        auto compiler = manager->getShaderCompiler();
        if (compiler) {
            std::string source(data.begin(), data.end());
            auto spv = compiler->compile(path, source, stage);
            if (spv) {
                return std::make_shared<vroom::ShaderAsset>(*spv, stage);
            }
        }
        return std::shared_ptr<vroom::ShaderAsset>(nullptr);
    });

    auto shader = manager->getAsset<vroom::ShaderAsset>("test.vert");
    ASSERT_NE(shader, nullptr);
    EXPECT_EQ(shader->getStage(), vroom::ShaderStage::Vertex);
    
    // Verify compiled content (reversed source)
    std::string expected = "}{ )(niam diov";
    std::string actual(shader->getData().begin(), shader->getData().end());
    EXPECT_EQ(actual, expected);
}

TEST_F(AssetManagerTest, PackageLoading) {
    // Register package provider
    manager->addProvider(std::make_unique<vroom::PackageAssetProvider>(testDir / "assets.vpk"));

    manager->registerLoader<TestAsset>([](const std::vector<char>& data, const std::string&) {
        auto asset = std::make_shared<TestAsset>();
        asset->content = std::string(data.begin(), data.end());
        return asset;
    });

    // Test root file in package
    auto asset1 = manager->getAsset<TestAsset>("pkg_file.txt");
    ASSERT_NE(asset1, nullptr);
    
    EXPECT_EQ(asset1->content, "Hello, Package!");

    // Test subdirectory file in package
    auto asset2 = manager->getAsset<TestAsset>("subdir/data.bin");
    ASSERT_NE(asset2, nullptr);
    EXPECT_EQ(asset2->content, "DATA");
}

TEST_F(AssetManagerTest, PackagePrecedence) {
    // Create a file on disk that conflicts with package file
    
    manager->clearProviders();
    
    // Create conflict file
    std::ofstream file(testDir / "pkg_file.txt");
    file << "Disk Override";
    file.close();

    // Case 1: Disk first
    manager->addProvider(std::make_unique<vroom::DiskAssetProvider>(testDir));
    manager->addProvider(std::make_unique<vroom::PackageAssetProvider>(testDir / "assets.vpk"));

    manager->registerLoader<TestAsset>([](const std::vector<char>& data, const std::string&) {
        auto asset = std::make_shared<TestAsset>();
        asset->content = std::string(data.begin(), data.end());
        return asset;
    });

    auto asset = manager->getAsset<TestAsset>("pkg_file.txt");
    ASSERT_NE(asset, nullptr);
    EXPECT_EQ(asset->content, "Disk Override");

    // Case 2: Package first (Simulate reloading/priority change)
    // Note: we must clear cache or use reloadAsset because "pkg_file.txt" is already cached!
    manager->clearProviders(); 
    manager->addProvider(std::make_unique<vroom::PackageAssetProvider>(testDir / "assets.vpk"));
    manager->addProvider(std::make_unique<vroom::DiskAssetProvider>(testDir)); // Fallback to disk

    // Force reload bypassing cache (or rely on clearProviders clearing cache which it does)
    auto asset2 = manager->getAsset<TestAsset>("pkg_file.txt");
    ASSERT_NE(asset2, nullptr);
    EXPECT_EQ(asset2->content, "Hello, Package!");
}
