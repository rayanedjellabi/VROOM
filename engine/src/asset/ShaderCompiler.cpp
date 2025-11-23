#include "vroom/asset/ShaderCompiler.hpp"
#include "vroom/logging/LogMacros.hpp"

#include <cstdio>
#include <fstream>
#include <array>
#include <sstream>
#include <iostream> // For popen/pclose

namespace vroom {

SystemShaderCompiler::SystemShaderCompiler() {
    // We could check if glslc is available here
}

std::optional<std::vector<char>> SystemShaderCompiler::compile(
    const std::filesystem::path& sourcePath,
    const std::string& sourceCode,
    ShaderStage stage
) {
    // 1. Write source to temporary file
    auto tempDir = std::filesystem::temp_directory_path();
    auto tempSourcePath = tempDir / ("vroom_shader_temp_" + std::to_string(std::hash<std::string>{}(sourcePath.string())) + ".glsl");
    auto tempSpvPath = tempSourcePath;
    tempSpvPath.replace_extension(".spv");

    {
        std::ofstream out(tempSourcePath);
        if (!out.is_open()) {
            LOG_ENGINE_ERROR("Failed to create temporary shader source file: " + tempSourcePath.string());
            return std::nullopt;
        }
        out << sourceCode;
    }

    // 2. Construct command
    std::string stageFlag;
    switch (stage) {
        case ShaderStage::Vertex: stageFlag = "vert"; break;
        case ShaderStage::Fragment: stageFlag = "frag"; break;
        case ShaderStage::Compute: stageFlag = "comp"; break;
        case ShaderStage::Geometry: stageFlag = "geom"; break;
        case ShaderStage::TessellationControl: stageFlag = "tesc"; break;
        case ShaderStage::TessellationEvaluation: stageFlag = "tese"; break;
        default: 
            LOG_ENGINE_ERROR("Unknown shader stage for compilation.");
            return std::nullopt;
    }

    std::string command = "glslc -fshader-stage=" + stageFlag + " -o \"" + tempSpvPath.string() + "\" \"" + tempSourcePath.string() + "\"";

    // 3. Execute command
    std::string output;
    bool success = false;
    
    // Using popen to capture output (cross-platform enough for Unix/macOS, might need adjustment for Windows)
    FILE* pipe = popen((command + " 2>&1").c_str(), "r");
    if (!pipe) {
        LOG_ENGINE_ERROR("Failed to run glslc command.");
        return std::nullopt;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    
    int returnCode = pclose(pipe);
    success = (returnCode == 0);

    if (!success) {
        LOG_ENGINE_ERROR("Shader compilation failed:\n" + output);
        // Clean up source
        std::filesystem::remove(tempSourcePath);
        return std::nullopt;
    }

    // 4. Read result
    std::ifstream spvFile(tempSpvPath, std::ios::binary | std::ios::ate);
    if (!spvFile.is_open()) {
        LOG_ENGINE_ERROR("Failed to read compiled SPIR-V file: " + tempSpvPath.string());
         std::filesystem::remove(tempSourcePath);
        return std::nullopt;
    }

    size_t size = spvFile.tellg();
    std::vector<char> spvData(size);
    spvFile.seekg(0, std::ios::beg);
    spvFile.read(spvData.data(), size);

    // 5. Cleanup
    std::filesystem::remove(tempSourcePath);
    std::filesystem::remove(tempSpvPath);

    return spvData;
}

} // namespace vroom


