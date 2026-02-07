#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Setup script for xdelta3 Python bindings
"""

from setuptools import setup, Extension
import os
import sys

# Determine source directory
xdelta3_dir = os.path.join(os.path.dirname(__file__), 'xdelta3')

# Check if we're on a system that supports pthreads
pthread_available = sys.platform != 'win32'

# Define compile flags
extra_compile_args = [
    '-std=c99',
    '-DREGRESSION_TEST=1',
    '-DSECONDARY_DJW=1',
    '-DSECONDARY_FGK=1',
    '-DXD3_MAIN=0',
    '-DXD3_DEBUG=0',
]

extra_link_args = []
libraries = ['m']

if pthread_available:
    extra_compile_args.append('-DXDELTA3_PARALLEL=1')
    libraries.append('pthread')

# SWIG extension module
xdelta3_module = Extension(
    '_xdelta3',
    sources=[
        os.path.join(xdelta3_dir, 'xdelta3.i'),
        os.path.join(xdelta3_dir, 'xdelta3.c'),
        os.path.join(xdelta3_dir, 'xdelta3-parallel.c'),
    ],
    include_dirs=[xdelta3_dir],
    libraries=libraries,
    extra_compile_args=extra_compile_args,
    extra_link_args=extra_link_args,
    swig_opts=['-I' + xdelta3_dir],
)

# Read the long description from README
long_description = """
Xdelta3 Python Bindings
=======================

Python bindings for xdelta3, a delta compression library using VCDIFF/RFC 3284 streams.

Features:
- Fast delta compression and decompression
- Multi-threaded support for improved performance
- Easy-to-use Python API
- Memory-efficient processing

Basic Usage:
```python
import xdelta3

# Encode (create patch)
patch = xdelta3.encode(new_data, old_data)

# Decode (apply patch)
reconstructed = xdelta3.decode(patch, old_data)
```
"""

setup(
    name='xdelta3',
    version='3.1.0',
    description='Python bindings for xdelta3 delta compression library',
    long_description=long_description,
    long_description_content_type='text/markdown',
    author='Joshua MacDonald',
    license='Apache License 2.0',
    url='https://github.com/xeonliu/xdelta',
    ext_modules=[xdelta3_module],
    py_modules=['xdelta3'],
    python_requires='>=2.7',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: Apache Software License',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: 3.11',
        'Programming Language :: C',
        'Topic :: System :: Archiving :: Compression',
        'Topic :: Software Development :: Libraries',
    ],
    keywords='xdelta delta compression diff patch vcdiff',
)
