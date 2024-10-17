#include <stdio.h>
#include <stdlib.h> // you can remove this if your code doesn't use it
#include <string.h> // you can remove this if your code doesn't use it
#include "quick.h"
#include <time.h>  // For random seed 	

/* Global variables to store the number of primitive operations */
size_t count_first, count_last, count_random, count_median;

/* Function Prototypes */
void quickFirst(void* base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
void quickLast(void* base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
void quickRandom(void* base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
void quickMedianOf3(void* base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
size_t partition(void* base, size_t nmemb, size_t size, int (*compar)(const void*, const void*), enum pivotType type);
size_t partition_helper(void* base, size_t nmemb, size_t size, int (*compar)(const void*, const void*));

void quick(void* base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) { 
    /* Copy base array for the four quick sorts */ 
    void *inputA = malloc(nmemb * size);
    void *inputB = malloc(nmemb * size);
    void *inputC = malloc(nmemb * size);
    /*Seed random number generator*/
    static int seeded = 0; // Set seeded flag to prevent reseeding
    if (!seeded) {
        srand((unsigned int)time(NULL)); 
        seeded = 1;  
    }
    memcpy(inputA, base, nmemb * size);
    memcpy(inputB, base, nmemb * size);
    memcpy(inputC, base, nmemb * size);

    /* First element as pivot */
    quickFirst(base, nmemb, size, compar);
    
    /* Last element as pivot */
    quickLast(inputA, nmemb, size, compar);

    /* Random element as pivot (run 10 times and average) */
    size_t total_random_count = 0;
    for (int i = 0; i < 10; i++) {
        /* Reset the input copy and count for each run */
        memcpy(inputB, base, nmemb * size);
        count_random = 0;
        
        /* Run quickRandom */
        quickRandom(inputB, nmemb, size, compar);
        
        /* Accumulate the count */
        total_random_count += count_random;
    }
    /* Calculate the average for Random */
    count_random = total_random_count / 10;

    /* Median-of-three as pivot */
    quickMedianOf3(inputC, nmemb, size, compar);

    /* Sanity Check: Compare results */
    for (size_t i = 0; i < nmemb; i++) {
        if (((int*)base)[i] != ((int*)inputA)[i] ||
            ((int*)base)[i] != ((int*)inputB)[i] ||
            ((int*)base)[i] != ((int*)inputC)[i]) {
            printf("Sanity check failed in quick\n");
            break;
        }
    }
    /* Printing the results */
    printf("First: %zu\n", count_first);
    printf("Last: %zu\n", count_last);
    printf("Random: %zu\n", count_random);
    printf("Median-of-three: %zu\n", count_median);

    /* Free the allocated memory */
    free(inputA);
    free(inputB);
    free(inputC);

}

size_t partition(void* base, size_t nmemb, size_t size, int (*compar)(const void*, const void*), enum pivotType type) {
    size_t pivotIndex = 0;

    switch(type) {
        case FIRST:
            count_first += nmemb - 1;
            pivotIndex = 0;
            break;
        case LAST:
            count_last += nmemb - 1;
            pivotIndex = nmemb - 1;
            break;
        case RANDOM:
            count_random += nmemb - 1;
            pivotIndex = rand() % nmemb;
            break;
        case MEDIANOF3:
            count_median += nmemb - 1;
            if (nmemb == 2) {
                // With two elements, simply return partition based on the first element.
                //printf("Only two elements. Selecting the first element as pivot.\n");
                return partition_helper(base, nmemb, size, compar);
            }

            count_median += nmemb - 1;
            size_t mid = nmemb / 2;
            size_t last = nmemb - 1;
            void *a = (char*)base;                    // First element
            void *b = (char*)base + mid * size;       // Middle element
            void *c = (char*)base + last * size;      // Last element

            /*
            printf("Comparing three elements:\n");
            printf("First element (a): %d\n", *(int*)a);
            printf("Middle element (b): %d\n", *(int*)b);
            printf("Last element (c): %d\n", *(int*)c); */

            if (compar(a, b) < 0) { // a < b
                if (compar(a, c) >= 0) { // c <= a < b
                    pivotIndex = 0;
                    count_median += 2;
                    //printf("Choosing 'a' as pivot. c <= a < b\n");
                } else if (compar(b, c) >= 0) { // a < c <= b
                    pivotIndex = last;
                    count_median += 3;
                    //printf("Choosing 'c' as pivot. a < c <= b\n");
                } else { // a < b < c
                    pivotIndex = mid;
                    count_median += 3;
                    //printf("Choosing 'b' as pivot. a < b < c\n");
                }
            } else { // b <= a
                if (compar(b, c) > 0) { // c < b <= a
                    pivotIndex = mid;
                    count_median += 2;
                    //printf("Choosing 'b' as pivot. c < b <= a\n");
                } else if (compar(a, c) <= 0) { // b <= a <= c
                    pivotIndex = 0;
                    count_median += 3;
                    //printf("Choosing 'a' as pivot. b <= a <= c\n");
                } else { // b <= c < a
                    pivotIndex = last;
                    count_median += 3;
                    //printf("Choosing 'c' as pivot. b <= c < a\n");
                }
            }

            //printf("Selected pivot index: %zu\n", pivotIndex);
            //printf("--------------------------------------------------\n");
            break;
    }
    /* Swap the chosen pivot with the first element */
    void *temp = malloc(size);
    memcpy(temp, (char*)base + pivotIndex * size, size);
    memcpy((char*)base + pivotIndex * size, base, size);
    memcpy(base, temp, size);
    free(temp);

    return partition_helper(base, nmemb, size, compar);
}


size_t partition_helper(void* base, size_t nmemb, size_t size, int (*compar)(const void*, const void*)) {
    size_t i = 1;
    char *pivot = base;
    void *temp = malloc(size);

    for (size_t j = 1; j < nmemb; j++) {
        if (compar((char*)base + j * size, pivot) < 0) {
            memcpy(temp, (char*)base + i * size, size);
            memcpy((char*)base + i * size, (char*)base + j * size, size);
            memcpy((char*)base + j * size, temp, size);
            i++;
        }
    }

    /* Put pivot at its correct position */
    memcpy(temp, pivot, size);
    memcpy(pivot, (char*)base + (i - 1) * size, size);
    memcpy((char*)base + (i - 1) * size, temp, size);
    free(temp);

    return i - 1;
}

void quickFirst(void* base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    if (nmemb <= 1) return;
    size_t pivot = partition(base, nmemb, size, compar, FIRST);
    quickFirst(base, pivot, size, compar);
    quickFirst((char*)base + (pivot + 1) * size, nmemb - pivot - 1, size, compar);
}

void quickLast(void* base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    if (nmemb <= 1) return;
    size_t pivot = partition(base, nmemb, size, compar, LAST);
    quickLast(base, pivot, size, compar);
    quickLast((char*)base + (pivot + 1) * size, nmemb - pivot - 1, size, compar);
}

void quickRandom(void* base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    if (nmemb <= 1) return;
    size_t pivot = partition(base, nmemb, size, compar, RANDOM);
    quickRandom(base, pivot, size, compar);
    quickRandom((char*)base + (pivot + 1) * size, nmemb - pivot - 1, size, compar);
}

void quickMedianOf3(void* base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    if (nmemb <= 1) return;
    size_t pivot = partition(base, nmemb, size, compar, MEDIANOF3);
    quickMedianOf3(base, pivot, size, compar);
    quickMedianOf3((char*)base + (pivot + 1) * size, nmemb - pivot - 1, size, compar);
}
