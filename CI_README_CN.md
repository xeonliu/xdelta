# CI/CD 自动构建说明

## 概述

本仓库已配置 GitHub Actions CI/CD 自动构建系统，可在每次代码提交时自动构建和测试 xdelta3。

## CI 工作流

### 1. 主构建工作流 (build.yml)

**触发条件：**
- 推送到 main、master 或开发分支
- Pull Request
- 手动触发

**测试平台：**
- ✅ **Ubuntu Linux** (最新版)
  - Python 3.8, 3.9, 3.10, 3.11, 3.12
- ✅ **macOS** (最新版)
  - Python 3.11
- ✅ **Windows** (最新版, MSVC 编译器)

**构建步骤：**
1. **安装依赖**
   - autoconf, automake, libtool
   - liblzma (压缩库)
   - SWIG (Python 绑定生成器)
   - Python 开发包

2. **构建 C 库**
   ```bash
   ./generate_build_files.sh
   ./configure
   make -j$(nproc)
   ```

3. **运行测试**
   - xdelta3regtest (回归测试)
   - parallel_test (并行功能测试)
   - 基本编码/解码测试

4. **构建 Python 绑定**
   ```bash
   python setup.py build
   ```

5. **测试 Python 模块**
   - 导入 _xdelta3 模块
   - 验证功能可用性

### 2. 快速检查工作流 (quick-check.yml)

**触发条件：**
- 除 main/master 外的所有分支推送
- Pull Request

**用途：**
- 为开发分支提供快速反馈
- 基本构建验证
- 简单编码/解码测试

**优点：**
- 运行时间短 (约 2-3 分钟)
- 快速发现明显错误

### 3. Python 包工作流 (python.yml)

**触发条件：**
- 推送到 main/master
- 版本标签 (v*)
- Pull Request

**功能：**
- 构建 Python wheel 包
- 测试多个 Python 版本 (3.8-3.12)
- 上传构建产物
- 验证包安装和导入

## 状态徽章

README 中已添加构建状态徽章：

[![Build and Test](https://github.com/xeonliu/xdelta/actions/workflows/build.yml/badge.svg)](https://github.com/xeonliu/xdelta/actions/workflows/build.yml)

点击徽章可查看详细的构建历史和日志。

## 查看 CI 结果

1. 访问 GitHub 仓库页面
2. 点击 "Actions" 标签
3. 选择工作流查看运行历史
4. 点击具体运行查看详细日志

## CI 构建内容

### C 库构建
- ✅ xdelta3 主程序
- ✅ xdelta3regtest (测试程序)
- ✅ xdelta3decode (解码器)
- ✅ xdelta3checksum (校验和工具)
- ✅ parallel_test (并行功能测试)

### Python 绑定
- ✅ _xdelta3 扩展模块
- ✅ Python 2/3 兼容性
- ✅ 多版本测试

### 示例程序
- ✅ encode_decode_test
- ✅ compare_test
- ✅ speed_test
- ✅ small_page_test

## 本地测试

如果要在本地复现 CI 的构建过程：

### Linux/macOS

```bash
# 安装依赖
# Ubuntu/Debian:
sudo apt-get install autoconf automake libtool liblzma-dev swig python3-dev

# macOS:
brew install autoconf automake libtool xz swig

# 构建 C 库
cd xdelta3
./generate_build_files.sh
./configure
make -j$(nproc)

# 运行测试
./xdelta3 -V
./xdelta3regtest
./parallel_test

# 构建 Python 绑定
cd ..
python setup.py build
```

### Windows

```bash
# 使用 Visual Studio
cd xdelta3
msbuild xdelta3.vcxproj /p:Configuration=Release /p:Platform=x64
```

## CI 配置文件位置

所有 CI 配置文件位于 `.github/workflows/` 目录：

```
.github/
└── workflows/
    ├── build.yml          # 主构建工作流
    ├── quick-check.yml    # 快速检查工作流
    └── python.yml         # Python 包工作流
```

## 自定义 CI

如需修改 CI 配置：

1. 编辑 `.github/workflows/` 中的 YAML 文件
2. 提交更改
3. CI 将在下次推送时使用新配置

## 问题排查

### 构建失败

1. 检查 Actions 标签页的详细日志
2. 查看具体失败步骤
3. 在本地复现问题
4. 修复后重新提交

### 测试失败

1. 查看测试输出
2. 检查是否是平台特定问题
3. 在对应平台上本地测试
4. 更新测试或代码

## 性能

各工作流的预期运行时间：

- **quick-check**: 2-3 分钟
- **build (Linux, 单个 Python 版本)**: 5-8 分钟
- **build (macOS)**: 8-12 分钟
- **build (Windows)**: 5-10 分钟
- **python**: 8-15 分钟

总体构建时间取决于 GitHub Actions 的可用资源。

## 未来改进

可能的 CI 增强：

- [ ] 添加代码覆盖率报告
- [ ] 添加静态代码分析
- [ ] 添加性能基准测试
- [ ] 添加文档自动生成
- [ ] 添加发布自动化

## 总结

✅ 自动构建系统已完全配置
✅ 支持多平台 (Linux, macOS, Windows)
✅ 支持多 Python 版本 (3.8-3.12)
✅ 自动运行测试
✅ 提供快速反馈
✅ 构建状态徽章

所有代码提交都将自动触发 CI 构建，确保代码质量！
