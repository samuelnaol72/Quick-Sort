#ifndef _QUICK_H_
#define _QUICK_H_

#ifndef _PIVOT_TYPES
#define _PIVOT_TYPES

// we've provided an pivotType enum you can use as a parameter when your define your quicksort function. 
enum pivotType {
  FIRST,
  LAST,
  RANDOM,
  MEDIANOF3
};

#endif

void quick(void* base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

#endif