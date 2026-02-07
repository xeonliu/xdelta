# CI/CD Documentation

## Overview

This repository is configured with GitHub Actions CI/CD for automated building and testing of xdelta3 on every commit.

## CI Workflows

### 1. Main Build Workflow (build.yml)

**Triggers:**
- Push to main, master, or development branches
- Pull requests
- Manual dispatch

**Test Platforms:**
- ✅ **Ubuntu Linux** (latest)
  - Python 3.8, 3.9, 3.10, 3.11, 3.12
- ✅ **macOS** (latest)
  - Python 3.11
- ✅ **Windows** (latest, MSVC)

**Build Steps:**
1. **Install Dependencies**
   - autoconf, automake, libtool
   - liblzma (compression library)
   - SWIG (Python bindings generator)
   - Python development packages

2. **Build C Library**
   ```bash
   ./generate_build_files.sh
   ./configure
   make -j$(nproc)
   ```

3. **Run Tests**
   - xdelta3regtest (regression tests)
   - parallel_test (parallel functionality)
   - Basic encode/decode tests

4. **Build Python Bindings**
   ```bash
   python setup.py build
   ```

5. **Test Python Module**
   - Import _xdelta3 module
   - Verify functionality

### 2. Quick Check Workflow (quick-check.yml)

**Triggers:**
- Pushes to all branches except main/master
- Pull requests

**Purpose:**
- Rapid feedback for development branches
- Basic build verification
- Simple encode/decode tests

**Benefits:**
- Fast execution (~2-3 minutes)
- Quick error detection

### 3. Python Package Workflow (python.yml)

**Triggers:**
- Push to main/master
- Version tags (v*)
- Pull requests

**Features:**
- Build Python wheels
- Test multiple Python versions (3.8-3.12)
- Upload build artifacts
- Verify package installation

## Build Status Badge

Build status badge added to README:

[![Build and Test](https://github.com/xeonliu/xdelta/actions/workflows/build.yml/badge.svg)](https://github.com/xeonliu/xdelta/actions/workflows/build.yml)

Click the badge to view detailed build history and logs.

## Viewing CI Results

1. Visit the GitHub repository page
2. Click the "Actions" tab
3. Select a workflow to view run history
4. Click on a specific run for detailed logs

## What Gets Built

### C Library
- ✅ xdelta3 main program
- ✅ xdelta3regtest (test suite)
- ✅ xdelta3decode (decoder)
- ✅ xdelta3checksum (checksum tool)
- ✅ parallel_test (parallel functionality test)

### Python Bindings
- ✅ _xdelta3 extension module
- ✅ Python 2/3 compatibility
- ✅ Multi-version testing

### Examples
- ✅ encode_decode_test
- ✅ compare_test
- ✅ speed_test
- ✅ small_page_test

## Local Testing

To reproduce CI builds locally:

### Linux/macOS

```bash
# Install dependencies
# Ubuntu/Debian:
sudo apt-get install autoconf automake libtool liblzma-dev swig python3-dev

# macOS:
brew install autoconf automake libtool xz swig

# Build C library
cd xdelta3
./generate_build_files.sh
./configure
make -j$(nproc)

# Run tests
./xdelta3 -V
./xdelta3regtest
./parallel_test

# Build Python bindings
cd ..
python setup.py build
```

### Windows

```bash
# Using Visual Studio
cd xdelta3
msbuild xdelta3.vcxproj /p:Configuration=Release /p:Platform=x64
```

## CI Configuration Files

All CI configuration files are in `.github/workflows/`:

```
.github/
└── workflows/
    ├── build.yml          # Main build workflow
    ├── quick-check.yml    # Quick check workflow
    └── python.yml         # Python package workflow
```

## Customizing CI

To modify CI configuration:

1. Edit YAML files in `.github/workflows/`
2. Commit changes
3. CI will use new configuration on next push

## Troubleshooting

### Build Failures

1. Check detailed logs in Actions tab
2. Look at specific failing step
3. Reproduce locally
4. Fix and recommit

### Test Failures

1. Review test output
2. Check for platform-specific issues
3. Test locally on affected platform
4. Update tests or code

## Performance

Expected run times for each workflow:

- **quick-check**: 2-3 minutes
- **build (Linux, single Python version)**: 5-8 minutes
- **build (macOS)**: 8-12 minutes
- **build (Windows)**: 5-10 minutes
- **python**: 8-15 minutes

Total build time depends on GitHub Actions available resources.

## Future Improvements

Possible CI enhancements:

- [ ] Add code coverage reporting
- [ ] Add static code analysis
- [ ] Add performance benchmarks
- [ ] Add documentation generation
- [ ] Add release automation

## Summary

✅ Automated build system fully configured
✅ Multi-platform support (Linux, macOS, Windows)
✅ Multi-version Python testing (3.8-3.12)
✅ Automated test execution
✅ Fast feedback
✅ Build status badge

All code commits automatically trigger CI builds to ensure code quality!
