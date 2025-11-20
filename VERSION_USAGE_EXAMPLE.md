# Version Usage Example

This document shows how to use the version information in your code.

## Including the Version Header

The version header is automatically generated during CMake configuration and can be included in your code:

```cpp
#include <vroom/core/Version.hpp>

// Use version information
std::cout << "VROOM Engine Version: " << vroom::Version::getVersionString() << std::endl;
std::cout << "Git Hash: " << vroom::Version::GIT_HASH << std::endl;

// Access version components
int major = vroom::Version::MAJOR;
int minor = vroom::Version::MINOR;
int patch = vroom::Version::PATCH;

// Get version as integer (MAJOR * 10000 + MINOR * 100 + PATCH)
int versionInt = vroom::Version::getVersionInt();
```

## Example Usage in Main

```cpp
#include <vroom/core/Version.hpp>
#include <iostream>

int main() {
    std::cout << "VROOM Engine " << vroom::Version::getVersionString() 
              << " (build " << vroom::Version::GIT_HASH << ")" << std::endl;
    
    // Your application code here...
    
    return 0;
}
```

## Version Detection

The version is automatically detected in the following order:

1. **CMake Variable**: If `VROOM_VERSION` is set (e.g., `-DVROOM_VERSION=1.2.3`), it uses that value
2. **Git Tag**: If the project is a git repository, it tries to get the latest tag
3. **Default**: Falls back to `0.0.0` if neither is available

## GitHub Releases

When creating a release on GitHub:
- The version is automatically extracted from the release tag name
- The tag can include a 'v' prefix (e.g., `v1.2.3`) which will be automatically removed
- The version is passed to CMake via `-DVROOM_VERSION` during the build

