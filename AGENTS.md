# AGENTS.md - Guidelines for Agentic Coding in Huffmunch

## Project Overview

Huffmunch is a generic lossless compression library for NES/6502 platforms. It consists of:
- **C++ compression library**: `huffmunch.cpp`, `huffmunch.h`, `main.cpp`
- **6502 assembly decompressor**: `huffmunch.s`, `huffmunch_c.s`
- **Example NES project**: `danger/` folder

## Build Commands

### Building the compressor (C++)

```bash
# Build with GCC (Linux/macOS)
make

# Clean build artifacts
make clean

# Build with custom compiler
CXX=clang++ make
```

### Building with Visual Studio (Windows)

```bash
msbuild huffmunch.sln -t:rebuild -property:Configuration=release
msbuild huffmunch.sln -t:rebuild -property:Configuration=debug
```

### Testing

The project uses an integration test in the `danger/test` directory:

```bash
# From the danger directory, using Python and cc65:
cd danger
python danger_prepare.py
danger_compress.bat

# Build and run the C test (requires cc65)
cd danger/test
# Build test.c with cc65, link with huffmunch_c.s
test.bat
```

There is no dedicated unit test framework. Tests verify decompression correctness by comparing decompressed output against the original data.

## Code Style Guidelines

### General Conventions

- **Language**: C++11 compatible
- **Indentation**: 2 spaces (no tabs)
- **Line endings**: Unix-style (LF)
- **Max line length**: No strict limit, but keep lines readable

### File Organization

```cpp
// File header comment with project name, author, year, URL
#include <cassert>
#include <cstdio>
// ... standard library includes (alphabetically within groups)

#include "huffmunch.h"  // project headers last
```

### Header Files

- Use `#pragma once` for include guards
- Public API in header files (`.h`)
- Implementation in `.cpp` files
- Keep public interface minimal

### Naming Conventions

| Element | Convention | Example |
|---------|------------|---------|
| Constants | `CONSTANT_CASE` or `kCamelCase` | `HUFFMUNCH_OK`, `VERSION_MAJOR` |
| Types | PascalCase | `HuffTree`, `MunchInput` |
| Functions | camelCase | `huffmunch_compress()` |
| Variables | camelCase or snake_case | `buffer_in`, `output_size` |
| Class members | `member_` prefix or camelCase | `buffer`, `bit` |
| Enums | PascalCase values | `HUFFMUNCH_SEARCH_WIDTH` |

### Type Usage

- Prefer `unsigned int` over `unsigned` for clarity
- Use fixed-width types from `<cstdint>` when size matters: `uint8_t`, `uint32_t`
- Common typedef: `typedef unsigned int uint;`
- Use `const` for immutable values

### Code Formatting

**Braces**:
```cpp
if (condition)
{
    // code
}
else
{
    // code
}
```

**Functions**:
```cpp
ReturnType function_name(Type param1, Type param2)
{
    // body
}
```

**Classes**:
```cpp
class ClassName
{
public:
    ClassName();
    // ...
private:
    // members
};
```

### Imports and Namespaces

- Use `using namespace std;` in `.cpp` files (after includes)
- Avoid `using namespace` in header files
- Use explicit `std::` prefix in headers when needed

### Error Handling

- Return error codes (defined as constants)
- Use `printf()` for user-facing output
- Use `DEBUG_OUT()` macro for debug traces (controlled by `HUFFMUNCH_DEBUG`)
- Throw `std::runtime_error` for internal errors that cannot be recovered

### Comments

- Use C++ `//` style comments
- Comment complex algorithms with brief explanation
- Document function parameters in headers
- Keep comments concise

### Debugging

The library has built-in debug flags defined in `huffmunch.h`:
```cpp
HUFFMUNCH_DEBUG_TREE      // Huffman tree output
HUFFMUNCH_DEBUG_MUNCH    // Compression progress
HUFFMUNCH_DEBUG_VERIFY    // Decompression verification
HUFFMUNCH_DEBUG_INTERNAL // Internal diagnostics
HUFFMUNCH_DEBUG_HEADER   // Header information
```

Enable via `huffmunch_debug(HUFFMUNCH_DEBUG_FULL);`

### Common Patterns

**Output parameters** (reference style):
```cpp
int function(unsigned char* output, unsigned int& output_size);
```

**Checking return values**:
```cpp
int result = huffmunch_compress(...);
if (result != HUFFMUNCH_OK)
{
    printf("error: %s\n", huffmunch_error_description(result));
    return result;
}
```

**Memory allocation**:
```cpp
unsigned char* buffer = (unsigned char*)malloc(size);
if (buffer == NULL)
{
    // handle error
}
```

## Adding New Features

1. Add public API to `huffmunch.h`
2. Implement in `huffmunch.cpp`
3. If CLI-related, add to `main.cpp`
4. Test with the danger project or create test data

## CI/CD

GitHub Actions workflow: `.github/workflows/huffmunch.yml`
- Builds on Windows with MSBuild
- Runs compression test
- Builds cc65 and runs decompression test
- Builds NES demo

## License

This project may be used, reused, and modified for any purpose. See README.md for full license details.
