#include <cstdio>
#include <cstdlib>
#include "huffmunch.h"

int main() {
    unsigned char* compressed = NULL;
    unsigned char* decompressed = NULL;
    unsigned int comp_size = 0;
    unsigned int decomp_size = 0;
    
    FILE* f = fopen("output.hfm", "rb");
    fseek(f, 0, SEEK_END);
    comp_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    compressed = (unsigned char*)malloc(comp_size);
    fread(compressed, 1, comp_size, f);
    fclose(f);
    
    // First get decompressed size
    int result = huffmunch_decompress(compressed, comp_size, NULL, decomp_size);
    if (result != HUFFMUNCH_OK && result != HUFFMUNCH_OUTPUT_OVERFLOW) {
        printf("Error getting size: %d\n", result);
        return 1;
    }
    
    decompressed = (unsigned char*)malloc(decomp_size);
    result = huffmunch_decompress(compressed, comp_size, decompressed, decomp_size);
    if (result != HUFFMUNCH_OK) {
        printf("Decompress error: %d\n", result);
        return 1;
    }
    
    // Compare with original
    FILE* orig = fopen("output-onlinetexttools.bin", "rb");
    fseek(orig, 0, SEEK_END);
    unsigned int orig_size = ftell(orig);
    fseek(orig, 0, SEEK_SET);
    
    if (orig_size != decomp_size) {
        printf("Size mismatch: orig=%d, decomp=%d\n", orig_size, decomp_size);
        return 1;
    }
    
    unsigned char* orig_data = (unsigned char*)malloc(orig_size);
    fread(orig_data, 1, orig_size, orig);
    fclose(orig);
    
    for (unsigned int i = 0; i < orig_size; i++) {
        if (orig_data[i] != decompressed[i]) {
            printf("Mismatch at byte %u: got %02X, expected %02X\n", i, decompressed[i], orig_data[i]);
            return 1;
        }
    }
    
    printf("Decompression verified OK: %u bytes\n", decomp_size);
    return 0;
}
