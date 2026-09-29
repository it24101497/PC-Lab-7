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

    /* 5. SCAN: Each process gets the cumulative sum up to its rank */
    long long prefix_sum = 0; 
    
    MPI_Scan(&local_sum, &prefix_sum, 1, MPI_LONG_LONG, MPI_SUM, MPI_COMM_WORLD);

    /* 6. Calculate sum of all chunks before this process */
    long long sum_before_me = prefix_sum - local_sum;
    
    /* Bonus Verification: Calculate expected prefix sum using K*(K+1)/2 */
    long long K = (long long)(rank + 1) * chunk_size;
    long long expected_prefix = K * (K + 1) / 2;

    /* 7. Every process prints its results */
    printf("  Rank %d: local_sum = %lld, sum_before_me = %lld, prefix_sum = %lld (Expected: %lld)\n", 
           rank, local_sum, sum_before_me, prefix_sum, expected_prefix);

    /* 8. The last rank holds the global total and prints the final verification */
    if (rank == size - 1) {
        double elapsed = MPI_Wtime() - start;
        long long global_expected = (long long)N * (N + 1) / 2;
        printf("\n[Scan] Final total sum = %lld\n", prefix_sum);
        printf("[Scan] Expected        = %lld\n", global_expected);
        printf("[Scan] Correct?        = %s\n", prefix_sum == global_expected ? "YES" : "NO");
        printf("[Scan] Time            = %.4f sec\n", elapsed);
    }

    /* Cleanup */
    free(local_chunk);
    if (rank == 0) {
        free(array);
    }
    
    MPI_Finalize();
    return 0;
}
