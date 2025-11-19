# VROOM Engine

**V**ulkan **R**eal-time **O**bject **O**utput **M**anager

[![CI](https://github.com/rayanedjellabi/VROOM/actions/workflows/ci.yml/badge.svg)](https://github.com/rayanedjellabi/VROOM/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/rayanedjellabi/VROOM/branch/main/graph/badge.svg)](https://codecov.io/gh/rayanedjellabi/VROOM)

A work-in-progress Entity Component System (ECS) game engine built on Vulkan, designed for high-performance real-time rendering and game development.

## Overview

VROOM is a modern C++20 game engine that leverages Vulkan's low-level graphics API . The engine is being developed with a focus on:

- **ECS Architecture**: Entity Component System for flexible and performant game object management
- **Vulkan Rendering**: Modern graphics API for cross-platform rendering
- **C# Scripting**: Optional Mono integration for C# scripting support
- **Editor Tools**: Built-in editor application for scene management and debugging

## Features

### ✅ Implemented

- None yet lol

## Requirements

### Build System
- CMake 3.24 or higher
- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)

### Dependencies
- **Vulkan SDK**: Required for Vulkan API support
- **GLFW 3.4**: Window and input management (fetched automatically)
- **GLM 0.9.9.8**: Mathematics library (fetched automatically)
- **ImGui (docking branch)**: Immediate mode GUI (fetched automatically)
- **nlohmann/json v3.11.3**: JSON parsing (fetched automatically)

### Optional Dependencies
- **Mono**: For C# scripting support
  - macOS ARM64: Install via `brew install mono`
  - macOS x86_64: Install Mono framework from [mono-project.com](https://www.mono-project.com/download/)

## Building

### Prerequisites

1. Install the Vulkan SDK for your platform:
   - **macOS**: Download from [vulkan.org](https://vulkan.lunarg.com/sdk/home)
   - **Linux**: Install via package manager or download SDK
   - **Windows**: Download installer from [vulkan.org](https://vulkan.lunarg.com/sdk/home)

2. Ensure CMake 3.24+ is installed

### Build Steps

```bash
# Clone the repository
git clone <repository-url>
cd VROOM-Engine

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
cmake --build .

# Or use make (on Unix-like systems)
make
```

## Development Status

⚠️ **This project is currently work in progress.**

## Contributing

Contributions are welcome! Please feel free to submit issues, feature requests, or pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


## Acknowledgments

- [Vulkan](https://www.vulkan.org/) - Graphics API
- [GLFW](https://www.glfw.org/) - Window management
- [GLM](https://github.com/g-truc/glm) - Mathematics library
- [ImGui](https://github.com/ocornut/imgui) - Immediate mode GUI
- [nlohmann/json](https://github.com/nlohmann/json) - JSON library

