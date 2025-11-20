# Release Build Setup

This document describes the automated release build system for VROOM Engine.

## Overview

When you create a release on GitHub, the workflow automatically:
1. Builds the project for multiple operating systems (Linux, macOS x64, macOS ARM64, Windows)
2. Packages each build into a downloadable archive
3. Uploads the builds to the GitHub release page
4. Extracts version information from the release tag

## Supported Platforms

- **Linux** (x64): Ubuntu 22.04
- **macOS** (x64): macOS 13 (Intel)
- **macOS** (ARM64): macOS 14 (Apple Silicon)
- **Windows** (x64): Windows Server 2022

## How to Create a Release

1. Go to your GitHub repository
2. Click on "Releases" → "Create a new release"
3. Choose or create a tag (e.g., `v1.0.0` or `1.0.0`)
4. Fill in the release title and description
5. Click "Publish release"

The workflow will automatically:
- Extract the version from the tag (removing 'v' prefix if present)
- Build for all platforms
- Upload the builds to the release page

## Version Format

The version should follow semantic versioning: `MAJOR.MINOR.PATCH`

Examples:
- `1.0.0`
- `v1.0.0` (the 'v' prefix will be automatically removed)
- `2.1.3`
- `v2.1.3`

## Using Version in Code

The version is automatically available in your code via the `Version.hpp` header:

```cpp
#include <vroom/core/Version.hpp>

// Get version string
std::string version = vroom::Version::getVersionString();

// Access components
int major = vroom::Version::MAJOR;
int minor = vroom::Version::MINOR;
int patch = vroom::Version::PATCH;

// Get git hash
const char* hash = vroom::Version::GIT_HASH;
```

See `VERSION_USAGE_EXAMPLE.md` for more details.

## Version Detection

The version is detected in this order:
1. **CMake Variable**: `-DVROOM_VERSION=1.2.3` (used by GitHub Actions)
2. **Git Tag**: Latest tag in the repository
3. **Default**: `0.0.0` if neither is available

## Build Artifacts

Each platform produces:
- **Linux/macOS**: `vroom-<VERSION>-<PLATFORM>.tar.gz`
- **Windows**: `vroom-<VERSION>-<PLATFORM>.zip`

Example artifact names:
- `vroom-1.0.0-linux-x64.tar.gz`
- `vroom-1.0.0-macos-x64.tar.gz`
- `vroom-1.0.0-macos-arm64.tar.gz`
- `vroom-1.0.0-windows-x64.zip`

## Prerequisites

The workflow automatically installs:
- CMake
- Vulkan SDK
- Platform-specific build tools and dependencies

No manual setup is required - the workflow handles all dependencies.

## Troubleshooting

### Build Fails
- Check that the tag name follows semantic versioning format
- Verify that all dependencies can be installed (Vulkan SDK, CMake, etc.)
- Check the GitHub Actions logs for specific error messages

### Version Not Detected
- Ensure the release tag follows the format `X.Y.Z` or `vX.Y.Z`
- The workflow uses `fetch-depth: 0` to get full git history for tag detection

### Artifacts Not Uploaded
- Ensure the workflow has `contents: write` permission (already configured)
- Check that the release was created (not drafted) - drafts don't trigger the workflow

## Local Development

To test version detection locally:

```bash
# Set version explicitly
cmake -B build -DVROOM_VERSION=1.2.3

# Or let it detect from git tag
git tag v1.2.3
cmake -B build
```

The version header will be generated at:
`build/include/vroom/core/Version.hpp`

