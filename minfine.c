#include <stdio.h>
#include <stdlib.h>

typedef struct job {
    int id;              // job # (1..n)
    int length;          // # of days to take to finish this job
    int fine;            // dollars per day delayed before staring on the job
} Job;

/*-----------------------------------------------------------------*/
int InWeightOrder(const void *A, const void *B)
{
    const Job *pA = (const Job *)A, *pB = (const Job *)B;
    int a, b;

    a = (pA->fine * pB->length);
    b = (pB->fine * pA->length);
    
    if (a == b) // so consistent when equal (fall back on job order)
        return (pA->id < pB->id) ? -1 : 1;
    return (a > b) ? -1 : 1;
}
/*-----------------------------------------------------------------*/
int 
main()
{
    Job *pjobs;
    int i, n;
    
    if (scanf("%d\n", &n) != 1) {
        fprintf(stderr, "Error in input\n");
        return -1;
    }

    pjobs = malloc(n *sizeof(Job));
    if (pjobs == NULL) {
        fprintf(stderr, "malloc() failed\n");
        return -1;
    }

    // get the input
    for (i = 0; i < n; i++) {
        if (scanf("%d %d\n", 
                  &pjobs[i].length, &pjobs[i].fine) != 2)
            break;  
        pjobs[i].id = i + 1; 
    }
    if (i != n) {
        fprintf(stderr, "wrong input!\n");
        free(pjobs);
        return -1;
    }
    /* Sort jobs by the InWeightOrder comparator */
    qsort(pjobs, n, sizeof(Job), InWeightOrder);
    int days_passed = 0;
    int total_fine = 0;
     /* compute and print out the sequence of jobs
        print out the total fine for that sequence */
    for (i = 0; i < n; i++) {
        printf("%d ", pjobs[i].id);
        total_fine += pjobs[i].fine * days_passed;
        days_passed += pjobs[i].length;
    }
    printf("\nTotal fine: %d\n", total_fine);
    free(pjobs);

}
