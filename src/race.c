#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[]) {

    int count = 0;
    int total = 1000000;
    #pragma omp parallel for
    for (int i=0; i<total; i++)
    {
        count++;
    }
    printf("Final count is: %d\n", count);
    printf("Should be:      %d\n", total);
}  