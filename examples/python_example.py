#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Example usage of xdelta3 Python bindings

This example demonstrates how to use the xdelta3 library for delta compression.
"""

import sys
import os

# Add parent directory to path for development
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

try:
    import xdelta3_wrapper as xdelta3
except ImportError:
    print("Error: xdelta3 module not installed. Please run: python setup.py install")
    sys.exit(1)


def example_basic():
    """Basic encoding and decoding example"""
    print("=" * 60)
    print("Example 1: Basic Delta Compression")
    print("=" * 60)
    
    # Original data
    old_data = b"The quick brown fox jumps over the lazy dog"
    print("Old data: {}".format(old_data.decode('utf-8')))
    
    # Modified data
    new_data = b"The quick brown fox jumps over the lazy cat"
    print("New data: {}".format(new_data.decode('utf-8')))
    
    # Create a patch
    print("\nCreating patch...")
    patch = xdelta3.encode(new_data, old_data)
    
    if patch:
        print("Patch size: {} bytes".format(len(patch)))
        print("Compression ratio: {:.2f}%".format(
            100.0 * len(patch) / len(new_data)))
        
        # Apply the patch
        print("\nApplying patch...")
        reconstructed = xdelta3.decode(patch, old_data)
        
        if reconstructed:
            print("Reconstructed: {}".format(reconstructed.decode('utf-8')))
            print("Success: {}".format(reconstructed == new_data))
        else:
            print("Error: Failed to decode patch")
    else:
        print("Error: Failed to create patch")
    
    print()


def main():
    """Run example"""
    print("\n")
    print("#" * 60)
    print("# xdelta3 Python Bindings - Example")
    print("#" * 60)
    print()
    
    example_basic()
    
    print("#" * 60)
    print("# Example completed!")
    print("#" * 60)
    print()


if __name__ == '__main__':
    main()
