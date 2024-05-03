/**
 * Gauss-Seidel implementation using pthreads.
 *
 *
 * Course: Advanced Computer Architecture, Uppsala University
 * Course Part: Lab assignment 3
 *
 * Original author: Frédéric Haziza <daz@it.uu.se>
 * Heavily modified by: Andreas Sandberg <andreas.sandberg@it.uu.se>
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdatomic.h>
#include <stdbool.h>

#include "gs_interface.h"



/**
 * Tell the startup code that we want run in parallel mode.
 */
const int gsi_is_parallel = 1;

/**
 * Thread data structure passed to the thread entry function.
 */
typedef struct {
        int thread_id;
        pthread_t thread;

        double error;

        /* TASK: Do you need any thread local state for synchronization? */
        atomic_bool* row_flags; // Array of flags for each row

} thread_info_t;

/** Define to enable debug mode */
#define DEBUG 1 /* 1 */

/** Debug output macro. Only active when DEBUG is non-0 */
#define dprintf(...)                            \
        if (DEBUG)                              \
                fprintf(stderr, __VA_ARGS__)

/** Vector with information about all active threads */
thread_info_t *threads = NULL;

/** The global error for the last iteration */
static double global_error;


/** Flag to indicate whether a row has been completed.*/
static atomic_bool* row_done = NULL;

//pthread_barrier_t iteration_barrier; // Declare the iteration_barrier variable
        

void
gsi_init()
{
        gs_verbose_printf("\t****  Initializing the  environment ****\n");

        threads = (thread_info_t *)malloc(gs_nthreads * sizeof(thread_info_t));
        if (!threads) {
                fprintf(stderr,
                        "Failed to allocate memory for thread information.\n");
                exit(EXIT_FAILURE);
        }

        row_done = (atomic_bool *)calloc(gs_size, sizeof(atomic_bool));
        if (!row_done) {
        fprintf(stderr, "Failed to allocate memory for row flags.\n");
        exit(EXIT_FAILURE);
        }

        /* Initialize global_error to something larger than the
         * tolerance to get the algorithm started */
        global_error = gs_tolerance + 1;

        /* TASK: Initialize global variables here */
        for (int i = 0; i < gs_size; i++) {
                atomic_init(&row_done[i], false);
        }
        // Initialize the iteration_barrier variable
        // if (pthread_barrier_init(&iteration_barrier, NULL, gs_nthreads) != 0) {
        //         fprintf(stderr, "Failed to initialize barrier\n");
        //         exit(EXIT_FAILURE);
        // }
}

void
gsi_finish()
{
        gs_verbose_printf("\t****  Cleaning environment ****\n");

        /* TASK: Be nice and cleanup the stuff you initialized in
         * gsi_init()
         */

        if (threads)
                free(threads);
        if (row_done)
                free(row_done);
        // Destroy the iteration_barrier variable
        //pthread_barrier_destroy(&iteration_barrier);
        

}

static void              
thread_sweep(int tid, int iter, int lbound, int rbound)
{
        threads[tid].error = 0.0;

        for (int row = 1; row < gs_size - 1; row++) {
                dprintf("%d: checking wait condition\n"
                        "\titeration: %i, row: %i\n",
                        tid,
                        iter, row);

                /* TASK: Wait for data to be available from the thread
                 * to the left */
                while (!atomic_load(&threads[tid - 1].row_flags[row]));
                // Spin until the left neighbor thread completes the row

                dprintf("%d: Starting on row: %d\n", tid, row);

                /* Update this thread's part of the matrix */
                for (int col = lbound; col < rbound; col++) {
                        double new_value = 0.25 * (
                                gs_matrix[GS_INDEX(row + 1, col)] +
                                gs_matrix[GS_INDEX(row - 1, col)] +
                                gs_matrix[GS_INDEX(row, col + 1)] +
                                gs_matrix[GS_INDEX(row, col - 1)]);
                        threads[tid].error +=
                                fabs(gs_matrix[GS_INDEX(row, col)] - new_value);
                        gs_matrix[GS_INDEX(row, col)] = new_value;
                }

                /* TASK: Tell the thread to the right that this thread
                 * is done with the row */
                /* Set flag to indicate row completion */
                atomic_store(&threads[tid].row_flags[row], true);
                //atomic_store(&threads[(tid + 1) % gs_nthreads].row_flags[row], true);
                dprintf("%d: row %d done\n", tid, row);
        }

}

/**
 * Computing routine for each thread
 */
static void *
thread_compute(void *_self)
{
        thread_info_t *self = (thread_info_t *)_self;
        const int tid = self->thread_id;

        int lbound = 0, rbound = 0;

        // /* TASK: Compute bounds for this thread */
        lbound = (gs_size - 2) / gs_nthreads * tid + 1;
        rbound = (gs_size - 2) / gs_nthreads * (tid + 1) + 1;

        /* TASK: Handle the case where gs_size - 2 is not divisible by
         * gs_nthreads */
        // float step = (float)(gs_width-2) / (float)gs_nthreads;
        // lbound = step * tid + 1;           // 0 + 1 = 1            1,5 + 1 = 2      3 + 1 = 4           4,5 + 1 = 5
        // rbound = step * (tid+1) + 1;       // 1,5 + 1 = 2          3 + 1 = 4        4,5 + 1 = 5         6 + 1 = 7

        gs_verbose_printf("%i: lbound: %i, rbound: %i\n",tid, lbound, rbound);

        for (int iter = 0; iter < gs_iterations && global_error > gs_tolerance; iter++) {
                dprintf("%i: Starting iteration %i\n", tid, iter);

                thread_sweep(tid, iter, lbound, rbound);


                /* Synchronize: Wait for all threads to complete their row */
                for (int row = 1; row < gs_size - 1; row++) {
                        while (!atomic_load(&self->row_flags[row])); // Spin until the flag is set
                }
                
                /* Reset flags for next iteration */
                for (int row = 1; row < gs_size - 1; row++) {
                        atomic_store(&self->row_flags[row], false);
                }
                 /* TASK: Update global error */
                /* Note: The reduction should only be done by one
                 * thread after all threads have updated their local
                 * errors */
                /* Hint: Which thread is guaranteed to complete its
                 * sweep last? */
                /* The last thread (tid == gs_nthreads - 1) is guaranteed to complete its sweep last */
                if (tid == gs_nthreads - 1) {
                        global_error = 0.0;
                        for (int t = 0; t < gs_nthreads; ++t) {
                                global_error += threads[t].error;
                        }
                }

                dprintf("%d: iteration %d done\n", tid, iter);
                /* TASK: Iteration barrier */
                //pthread_barrier_wait(&iteration_barrier); // Wait for all threads to reach the barrier 

        }

        gs_verbose_printf(
                "\t****  Thread %d done after %d iterations ****\n",
                tid, gs_iterations);

        return NULL;
}

/**
 * Parallel implementation of the GS algorithm. Called from
 * gs_common.c to start the solver.
 */
void
gsi_calculate()
{
        int err;

        /* Allocate and initialize row flags for each thread */
        for (int t = 0; t < gs_nthreads; ++t) {
                threads[t].thread_id = t;
                threads[t].row_flags = (atomic_bool*)malloc(gs_size * sizeof(atomic_bool));
                if (!threads[t].row_flags) {
                        fprintf(stderr, "Failed to allocate memory for row flags.\n");
                        exit(EXIT_FAILURE);
                }
                for (int i = 0; i < gs_size; ++i) {
                        atomic_init(&threads[t].row_flags[i], false);
                }
        }

        /*Create threads*/
        for (int t = 0; t < gs_nthreads; t++) {
                gs_verbose_printf("\tSpawning thread %d\n",t);

                threads[t].thread_id = t;
                err = pthread_create(&threads[t].thread, NULL,thread_compute, &threads[t]);

                if (err) {
                        fprintf(stderr,
                                "Error: pthread_create() failed: %d, "
                                "thread %d\n",
                                err, t);
                        exit(EXIT_FAILURE);
                }
        }
  
        /* Calling pthread_join on a thread will block until the
         * thread terminates. Since we are joining all threads, we
         * wait until all threads have exited. */
        for (int t = 0; t < gs_nthreads; t++) {
                err = pthread_join(threads[t].thread, NULL);
                if (err) {
                        fprintf(stderr,
                                "Error: pthread_join() failed: %d, "
                                "thread %d\n",
                                err, t);
                        exit(EXIT_FAILURE);
                }
        }

        if (global_error <= gs_tolerance) {
                printf("Solution converged!\n");
        } else {
                printf("Reached maximum number of iterations. Solution did "
                       "NOT converge.\n");
                printf("Note: This is normal if you are using the "
                       "default settings.\n");
        }
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 8
 * indent-tabs-mode: nil
 * c-file-style: "linux"
 * End:
 */
