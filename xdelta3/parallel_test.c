/* Simple test for xdelta3 parallel functions */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/* Define configuration before xdelta3.h */
#define XD3_USE_LARGEFILE64 1
#define XD3_USE_LARGESIZET 1
#define SIZEOF_SIZE_T 8
#define SIZEOF_UNSIGNED_LONG 8  
#define SIZEOF_UNSIGNED_LONG_LONG 8
#define SIZEOF_UNSIGNED_INT 4
#define static_assert(x, y)

/* Minimal build - just encoder/decoder */
#define REGRESSION_TEST 0
#define SECONDARY_DJW 0
#define SECONDARY_FGK 0
#define XD3_MAIN 0

#include "xdelta3.h"
#include "xdelta3.c"
#include "xdelta3-parallel.h"
#include "xdelta3-parallel.c"

int main() {
    const char *old_data = "Hello World";
    const char *new_data = "Hello Beautiful World";
    
    size_t old_len = strlen(old_data);
    size_t new_len = strlen(new_data);
    
    // Allocate buffers
    size_t max_patch_size = new_len + old_len + 1024;
    uint8_t *patch = malloc(max_patch_size);
    usize_t patch_size = max_patch_size;
    
    size_t max_output_size = new_len + 1024;
    uint8_t *output = malloc(max_output_size);
    usize_t output_size = max_output_size;
    
    printf("Testing xdelta3 parallel functions...\n");
    printf("Old data: %s\n", old_data);
    printf("New data: %s\n", new_data);
    
    // Test encoding
    printf("\nTesting parallel encode...\n");
    int ret = xd3_encode_parallel(
        (const uint8_t *)new_data, new_len,
        (const uint8_t *)old_data, old_len,
        patch, &patch_size, max_patch_size,
        0, 2
    );
    
    if (ret != 0) {
        printf("ERROR: Parallel encode failed with code %d\n", ret);
        free(patch);
        free(output);
        return 1;
    }
    
    printf("Patch created successfully! Size: %lu bytes\n", (unsigned long)patch_size);
    
    // Test decoding
    printf("\nTesting parallel decode...\n");
    ret = xd3_decode_parallel(
        patch, patch_size,
        (const uint8_t *)old_data, old_len,
        output, &output_size, max_output_size,
        0, 2
    );
    
    if (ret != 0) {
        printf("ERROR: Parallel decode failed with code %d\n", ret);
        free(patch);
        free(output);
        return 1;
    }
    
    printf("Decode successful! Size: %lu bytes\n", (unsigned long)output_size);
    
    // Verify result
    if (output_size == new_len && memcmp(output, new_data, new_len) == 0) {
        printf("\nSUCCESS: Decoded data matches original!\n");
        free(patch);
        free(output);
        return 0;
    } else {
        printf("\nERROR: Decoded data doesn't match!\n");
        printf("Expected: %s\n", new_data);
        printf("Got: %.*s\n", (int)output_size, output);
        free(patch);
        free(output);
        return 1;
    }
}
