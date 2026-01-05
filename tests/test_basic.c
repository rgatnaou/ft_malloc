#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    printf("=== Testing Basic Malloc ===\n");
    
    // Test 1: Simple allocation
    char *str = malloc(100);
    if (str == NULL) {
        printf("FAIL: malloc returned NULL\n");
        return 1;
    }
    
    strcpy(str, "Hello, World!");
    printf("Test 1 PASS: %s\n", str);
    
    // Test 2: Multiple allocations
    int *numbers = malloc(10 * sizeof(int));
    if (numbers == NULL) {
        printf("FAIL: malloc for numbers returned NULL\n");
        return 1;
    }
    
    for (int i = 0; i < 10; i++) {
        numbers[i] = i * 10;
    }
    printf("Test 2 PASS: numbers[5] = %d\n", numbers[5]);
    
    // Test 3: Free
    free(str);
    free(numbers);
    printf("Test 3 PASS: Memory freed\n");
    
    // Test 4: realloc
    int *arr = malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++) arr[i] = i;
    
    arr = realloc(arr, 10 * sizeof(int));
    if (arr == NULL) {
        printf("FAIL: realloc returned NULL\n");
        return 1;
    }
    
    printf("Test 4 PASS: realloc worked\n");
    for (int i = 5; i < 10; i++) arr[i] = i;
    
    free(arr);
    
    printf("\n=== All Basic Tests Passed ===\n");
    return 0;
}