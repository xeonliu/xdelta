#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
High-level Python API for xdelta3

This module provides easy-to-use functions for delta compression using xdelta3.
"""

import os
import sys

# Import the low-level SWIG bindings
try:
    from . import _xdelta3 as _xd3
except ImportError:
    import _xdelta3 as _xd3

__version__ = '3.1.0'
__all__ = ['encode', 'decode', 'encode_file', 'decode_file']

# Detect number of CPUs for parallel processing
try:
    import multiprocessing
    DEFAULT_THREADS = multiprocessing.cpu_count()
except (ImportError, NotImplementedError):
    DEFAULT_THREADS = 1


def encode(input_data, source_data=None, flags=0, num_threads=None):
    """
    Encode (compress) data using xdelta3.
    
    Creates a delta/patch from input_data relative to source_data.
    
    Args:
        input_data (bytes): The new/target data to encode
        source_data (bytes, optional): The old/source data to compare against.
                                      If None, creates a standalone compressed file.
        flags (int, optional): Compression flags (default: 0)
        num_threads (int, optional): Number of threads for parallel processing.
                                    Defaults to CPU count.
    
    Returns:
        bytes: The encoded delta/patch data, or None on error
    
    Example:
        >>> old_data = b"Hello World"
        >>> new_data = b"Hello Python World"
        >>> patch = encode(new_data, old_data)
    """
    if not isinstance(input_data, bytes):
        if sys.version_info[0] >= 3:
            input_data = input_data.encode('utf-8')
        else:
            input_data = str(input_data)
    
    if source_data is None:
        source_data = b''
    elif not isinstance(source_data, bytes):
        if sys.version_info[0] >= 3:
            source_data = source_data.encode('utf-8')
        else:
            source_data = str(source_data)
    
    # Estimate output size (input size + some overhead)
    max_output = len(input_data) + len(source_data) + 1024
    
    # Try to use parallel encoding if available and beneficial
    if num_threads is None:
        num_threads = DEFAULT_THREADS if len(input_data) > 1024 * 1024 else 1
    
    try:
        # Check if parallel functions are available
        if num_threads > 1 and hasattr(_xd3, 'xd3_encode_parallel'):
            result = _xd3.xd3_encode_parallel(
                input_data, len(input_data),
                source_data, len(source_data),
                max_output, flags, num_threads
            )
        else:
            result = _xd3.xd3_encode_memory(
                input_data, len(input_data),
                source_data, len(source_data),
                max_output, flags
            )
        
        # SWIG returns a tuple (status_code, output_data)
        if isinstance(result, tuple):
            status, output = result
            return output if status == 0 else None
        return None
    except Exception as e:
        import warnings
        warnings.warn("Encoding failed: {}".format(str(e)))
        return None


def decode(patch_data, source_data=None, flags=0, num_threads=None):
    """
    Decode (decompress/apply patch) data using xdelta3.
    
    Reconstructs the original data by applying patch_data to source_data.
    
    Args:
        patch_data (bytes): The patch/delta data
        source_data (bytes, optional): The old/source data. If None, treats
                                      patch as standalone compressed data.
        flags (int, optional): Decompression flags (default: 0)
        num_threads (int, optional): Number of threads for parallel processing.
                                    Defaults to CPU count.
    
    Returns:
        bytes: The decoded/reconstructed data, or None on error
    
    Example:
        >>> old_data = b"Hello World"
        >>> new_data = b"Hello Python World"
        >>> patch = encode(new_data, old_data)
        >>> reconstructed = decode(patch, old_data)
        >>> assert reconstructed == new_data
    """
    if not isinstance(patch_data, bytes):
        if sys.version_info[0] >= 3:
            patch_data = patch_data.encode('utf-8')
        else:
            patch_data = str(patch_data)
    
    if source_data is None:
        source_data = b''
    elif not isinstance(source_data, bytes):
        if sys.version_info[0] >= 3:
            source_data = source_data.encode('utf-8')
        else:
            source_data = str(source_data)
    
    # Estimate output size
    max_output = max(len(patch_data) * 10, len(source_data) * 2, 1024 * 1024)
    
    # Try to use parallel decoding if available and beneficial
    if num_threads is None:
        num_threads = DEFAULT_THREADS if len(patch_data) > 1024 * 1024 else 1
    
    try:
        # Check if parallel functions are available
        if num_threads > 1 and hasattr(_xd3, 'xd3_decode_parallel'):
            result = _xd3.xd3_decode_parallel(
                patch_data, len(patch_data),
                source_data, len(source_data),
                max_output, flags, num_threads
            )
        else:
            result = _xd3.xd3_decode_memory(
                patch_data, len(patch_data),
                source_data, len(source_data),
                max_output, flags
            )
        
        # SWIG returns a tuple (status_code, output_data)
        if isinstance(result, tuple):
            status, output = result
            return output if status == 0 else None
        return None
    except Exception as e:
        import warnings
        warnings.warn("Decoding failed: {}".format(str(e)))
        return None


def encode_file(input_file, source_file=None, output_file=None, 
                flags=0, num_threads=None):
    """
    Encode files using xdelta3.
    
    Args:
        input_file (str): Path to the new/target file
        source_file (str, optional): Path to the old/source file
        output_file (str, optional): Path for output patch file.
                                    If None, returns the patch data.
        flags (int, optional): Compression flags (default: 0)
        num_threads (int, optional): Number of threads for parallel processing
    
    Returns:
        bytes or bool: If output_file is None, returns patch data.
                      Otherwise returns True on success, False on failure.
    """
    with open(input_file, 'rb') as f:
        input_data = f.read()
    
    if source_file:
        with open(source_file, 'rb') as f:
            source_data = f.read()
    else:
        source_data = None
    
    patch = encode(input_data, source_data, flags, num_threads)
    
    if patch is None:
        return False if output_file else None
    
    if output_file:
        with open(output_file, 'wb') as f:
            f.write(patch)
        return True
    
    return patch


def decode_file(patch_file, source_file=None, output_file=None,
                flags=0, num_threads=None):
    """
    Decode files using xdelta3.
    
    Args:
        patch_file (str): Path to the patch/delta file
        source_file (str, optional): Path to the old/source file
        output_file (str, optional): Path for output file.
                                    If None, returns the decoded data.
        flags (int, optional): Decompression flags (default: 0)
        num_threads (int, optional): Number of threads for parallel processing
    
    Returns:
        bytes or bool: If output_file is None, returns decoded data.
                      Otherwise returns True on success, False on failure.
    """
    with open(patch_file, 'rb') as f:
        patch_data = f.read()
    
    if source_file:
        with open(source_file, 'rb') as f:
            source_data = f.read()
    else:
        source_data = None
    
    decoded = decode(patch_data, source_data, flags, num_threads)
    
    if decoded is None:
        return False if output_file else None
    
    if output_file:
        with open(output_file, 'wb') as f:
            f.write(decoded)
        return True
    
    return decoded


# Backwards compatibility alias
def compress(data, source=None):
    """Alias for encode() for backwards compatibility"""
    return encode(data, source)


def decompress(data, source=None):
    """Alias for decode() for backwards compatibility"""
    return decode(data, source)
