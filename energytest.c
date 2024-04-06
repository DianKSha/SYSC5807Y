/////// You need to compile the code of each experiment separately. If you want to compile them into library files separately (for example, static library.a or dynamic library.so), make sure that the compilation command contains all the necessary source files.

/// When compiling the test program, link to the corresponding library according to the version tested as needed. For example, if you use gcc, the command may look like this:
///gcc -o test_program test_program.c -L/path/to/original/library -loriginal -I/path/to/headers

//// Or, in order to test the modified version:
///gcc -o test_program test_program.c -L/path/to/modified/library -lmodified -I/path/to/headers


//////Adjust the test code

/// In the test program, make sure that you include the my_aead_module.h header file. Before running the test, adjust the linked library appropriately according to the experimental version (original or modified) you want to test.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "my_aead_module.h" 

// Structure to hold a single test vector
typedef struct {
    unsigned char* key;
    unsigned char* nonce;
    unsigned char* plaintext;
    unsigned char* ad;
    unsigned char* expected_ciphertext;
    unsigned char* expected_tag;
    size_t key_len;
    size_t nonce_len;
    size_t pt_len;
    size_t ad_len;
    size_t ct_len;
    size_t tag_len;
} TestVector;

// Function to load test vectors from a file
// NOTE: This is a simplified placeholder. You will need to parse the KAT file format.
void load_kat_file(const char* filename, TestVector** vectors, size_t* num_vectors) {
    // Implementation depends on the format of your KAT file
}

// Function to measure execution time of a single test vector
double measure_execution_time(TestVector vector, void (*encrypt_func)(const unsigned char*, size_t, const unsigned char*, size_t, const unsigned char*, size_t, const unsigned char*, size_t, unsigned char**, unsigned char**)) {
    clock_t start, end;
    double cpu_time_used;
    unsigned char* ciphertext;
    unsigned char* tag;

    start = clock();
    encrypt_func(vector.key, vector.key_len, vector.nonce, vector.nonce_len, vector.plaintext, vector.pt_len, vector.ad, vector.ad_len, &ciphertext, &tag);
    end = clock();

    // Add correctness checks here
    // For simplicity, assuming these functions exist and work as expected
    assert(correctness_check(ciphertext, vector.expected_ciphertext, vector.ct_len));
    assert(correctness_check(tag, vector.expected_tag, vector.tag_len));

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    free(ciphertext);
    free(tag);
    return cpu_time_used;
}

int main() {
    TestVector* test_vectors;
    size_t num_vectors;
    double total_time_original = 0, total_time_modified = 0;
    double avg_time_original, avg_time_modified;

    load_kat_file("LWC_AEAD_KAT_160_128.txt", &test_vectors, &num_vectors);

    for (size_t i = 0; i < num_vectors; i++) {
        total_time_original += measure_execution_time(test_vectors[i], original_encrypt);
        total_time_modified += measure_execution_time(test_vectors[i], modified_encrypt);
    }

    avg_time_original = total_time_original / num_vectors;
    avg_time_modified = total_time_modified / num_vectors;

    printf("Original Avg Time: %f seconds\n", avg_time_original);
    printf("Modified Avg Time: %f seconds\n", avg_time_modified);

    // Cleanup
    free(test_vectors); // Remember to free other dynamically allocated memory inside test_vectors

    return 0;
}
