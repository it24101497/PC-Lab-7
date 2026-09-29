#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk_size = N / size;
    int *array = NULL;

    /* 1. Root allocates full array */
    if (rank == 0) {
        array = (int *)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++)
            array[i] = i + 1;
        printf("Root filled array with values 1 to %d\n", N);
    }

    /* 2. All processes allocate their chunk */
    int *local_chunk = (int *)malloc(chunk_size * sizeof(int));

    double start = MPI_Wtime();

    /* 3. SCATTER: Distribute chunks */
    MPI_Scatter(array, chunk_size, MPI_INT, 
                local_chunk, chunk_size, MPI_INT, 
                0, MPI_COMM_WORLD);

    /* 4. Compute local sum */
    long long local_sum = 0;
    for (int i = 0; i < chunk_size; i++) {
        local_sum += local_chunk[i];
    }

    /* 5. ALLREDUCE: Every process receives the calculated sum of all local_sums */
    long long total_sum = 0; 
    
    /* Notice there is no root parameter (0) here like there was in MPI_Reduce */
    MPI_Allreduce(&local_sum, &total_sum, 1, MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);

    /* 6. Every process prints its own contribution since they all have total_sum */
    double percentage = ((double)local_sum / (double)total_sum) * 100.0;
    printf("  Rank %d: local_sum = %lld, total_sum = %lld, contribution = %.2f%%\n", 
           rank, local_sum, total_sum, percentage);

    /* 7. Root prints the final verification */
    if (rank == 0) {
        double elapsed = MPI_Wtime() - start;
        long long expected = (long long)N * (N + 1) / 2;
        printf("\n[Allreduce] Total sum   = %lld\n", total_sum);
        printf("[Allreduce] Expected    = %lld\n", expected);
        printf("[Allreduce] Correct?    = %s\n", total_sum == expected ? "YES" : "NO");
        printf("[Allreduce] Time        = %.4f sec\n", elapsed);
    }

    /* Cleanup */
    free(local_chunk);
    if (rank == 0) {
        free(array);
    }
    
    MPI_Finalize();
    return 0;
}
