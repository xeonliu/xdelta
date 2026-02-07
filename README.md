# Xdelta

Xdelta version 3 is a C library and command-line tool for delta
compression using VCDIFF/RFC 3284 streams.

# License

This repository contains branches of Xdelta 3.x that were
re-licensed by the original author under the [Apache Public
License version 2.0](http://www.apache.org/licenses/LICENSE-2.0),
namely:

- __release3_0_apl__ Change to APL based on 3.0.11 sources
- __release3_1_apl__ Merges release3_0_apl with 3.1.0 sources

The original GPL licensed Xdelta lives at http://github.com/jmacd/xdelta-gpl.

# Documentation

See the [command-line usage](https://github.com/jmacd/xdelta/blob/wiki/CommandLineSyntax.md).  See [wiki directory](https://github.com/jmacd/xdelta/tree/wiki).

# Python Bindings

This repository includes Python bindings for xdelta3 with multi-threading support. See [PYTHON_README.md](PYTHON_README.md) for installation and usage instructions.

Quick example:
```python
import xdelta3_wrapper as xdelta3

# Create and apply patches easily
patch = xdelta3.encode(new_data, old_data)
reconstructed = xdelta3.decode(patch, old_data)
```

Install:
```bash
python setup.py install
```

# Multi-core Support

The library now includes multi-threaded processing support for improved performance on multi-core systems. The parallel processing infrastructure is available through both the C API and Python bindings.




