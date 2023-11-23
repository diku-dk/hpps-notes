#include <pthread.h>
#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[]) {

    int count = 0;
    int total = 1000000;
    pthread_mutex_t lock; 

    pthread_mutex_init(&lock, NULL);

    #pragma omp parallel for
    for (int i=0; i<total; i++)
    {
        pthread_mutex_lock(&lock); 
        count++;
        pthread_mutex_unlock(&lock); 
    }

    printf("Final count is: %d\n", count);
    printf("Should be:      %d\n", total);
}  