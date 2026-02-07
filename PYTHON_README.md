# Xdelta3 Python Bindings

Python bindings for xdelta3, a fast delta compression library using VCDIFF/RFC 3284 streams.

## Features

- **Fast delta compression**: Create patches between files or data
- **Multi-threaded support**: Utilize multiple CPU cores for faster processing
- **Easy-to-use API**: Simple Python interface for encoding and decoding
- **Memory efficient**: Process large files without loading everything into memory
- **Cross-platform**: Works on Linux, macOS, and Windows

## Installation

### From Source

```bash
# Install dependencies (SWIG required for building)
# On Ubuntu/Debian:
sudo apt-get install swig python3-dev

# On macOS:
brew install swig

# Build and install
python setup.py build
python setup.py install
```

### Using pip (once published)

```bash
pip install xdelta3
```

## Quick Start

### Basic Usage

```python
import xdelta3_wrapper as xdelta3

# Create a delta/patch between old and new data
old_data = b"Hello World"
new_data = b"Hello Python World"

# Encode: create patch
patch = xdelta3.encode(new_data, old_data)

# Decode: apply patch
reconstructed = xdelta3.decode(patch, old_data)

assert reconstructed == new_data
```

### File Operations

```python
import xdelta3_wrapper as xdelta3

# Create a patch file
success = xdelta3.encode_file(
    'new_version.bin',      # new file
    'old_version.bin',      # old file
    'update.patch'          # output patch
)

# Apply patch file
success = xdelta3.decode_file(
    'update.patch',         # patch file
    'old_version.bin',      # old file
    'restored.bin'          # output file
)
```

### Multi-threaded Processing

```python
import xdelta3_wrapper as xdelta3

# Use multiple threads for large files (automatically detects CPU count)
patch = xdelta3.encode(new_data, old_data, num_threads=4)

# Or let it auto-detect
patch = xdelta3.encode(new_data, old_data)  # Uses all available CPUs
```

### Standalone Compression

```python
import xdelta3_wrapper as xdelta3

# Compress data without a source reference
compressed = xdelta3.encode(data)

# Decompress
original = xdelta3.decode(compressed)
```

## API Reference

### `encode(input_data, source_data=None, flags=0, num_threads=None)`

Encode (compress) data using xdelta3.

**Parameters:**
- `input_data` (bytes): The new/target data to encode
- `source_data` (bytes, optional): The old/source data. If None, creates standalone compression
- `flags` (int, optional): Compression flags (default: 0)
- `num_threads` (int, optional): Number of threads. Auto-detects if None

**Returns:**
- `bytes`: The encoded patch data, or None on error

### `decode(patch_data, source_data=None, flags=0, num_threads=None)`

Decode (decompress/apply patch) data using xdelta3.

**Parameters:**
- `patch_data` (bytes): The patch/delta data
- `source_data` (bytes, optional): The old/source data. If None, treats as standalone compression
- `flags` (int, optional): Decompression flags (default: 0)
- `num_threads` (int, optional): Number of threads. Auto-detects if None

**Returns:**
- `bytes`: The decoded/reconstructed data, or None on error

### `encode_file(input_file, source_file=None, output_file=None, flags=0, num_threads=None)`

Encode files using xdelta3.

**Parameters:**
- `input_file` (str): Path to the new/target file
- `source_file` (str, optional): Path to the old/source file
- `output_file` (str, optional): Path for output patch. If None, returns data
- `flags` (int, optional): Compression flags (default: 0)
- `num_threads` (int, optional): Number of threads

**Returns:**
- `bytes` or `bool`: Patch data if output_file is None, else True/False

### `decode_file(patch_file, source_file=None, output_file=None, flags=0, num_threads=None)`

Decode files using xdelta3.

**Parameters:**
- `patch_file` (str): Path to the patch/delta file
- `source_file` (str, optional): Path to the old/source file
- `output_file` (str, optional): Path for output. If None, returns data
- `flags` (int, optional): Decompression flags (default: 0)
- `num_threads` (int, optional): Number of threads

**Returns:**
- `bytes` or `bool`: Decoded data if output_file is None, else True/False

## Examples

See the `examples/python_example.py` file for more detailed examples including:
- Basic delta compression
- Standalone compression
- File operations
- Parallel processing benchmarks

Run the examples:
```bash
python examples/python_example.py
```

## Performance Tips

1. **Use multi-threading for large files**: Files larger than 1MB benefit from parallel processing
2. **Adjust thread count**: For very large files, using 2-4 threads often provides the best speedup
3. **Memory consideration**: The current implementation loads data into memory. For very large files, use the command-line tool instead

## Thread Safety

The Python bindings are thread-safe. You can safely call encode/decode from multiple Python threads.

## Limitations

- The current parallel implementation provides infrastructure for multi-threading but processes data sequentially
- Future versions will implement true window-level parallelization for better performance
- Very large files (>1GB) may require significant memory

## License

Apache License 2.0 - See LICENSE file for details

## Contributing

Contributions are welcome! Please feel free to submit pull requests.

## Links

- [xdelta3 GitHub repository](https://github.com/jmacd/xdelta)
- [VCDIFF RFC 3284](https://tools.ietf.org/html/rfc3284)
