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
        volatile int c_row; // Keep track of which row in the matrix the thread is in, visible to other threads and read from memory each time it is used instead of cached calue in a register.
	double padding[16]; // unused data to avoid false sharing.
    //ach thread has its own thread_info_t structure, and these structures are allocated contiguously in memory for better cache locality. However, without padding, adjacent elements in these structures could reside on the same cache line. This means that if one thread modifies its error field, it could inadvertently invalidate the cache line containing the error field of another thread, even though they are independent.

    //To avoid this, we add padding to the thread_info_t structure to ensure that each element resides on its own seperate cache line. This way, threads can modify their own error fields without affecting other threads.
} thread_info_t;

/** Define to enable debug mode */
#define DEBUG 0 /* 1 */

/** Debug output macro. Only active when DEBUG is non-0 */
#define dprintf(...)                            \
        if (DEBUG)                              \
                fprintf(stderr, __VA_ARGS__)

/** Vector with information about all active threads */
thread_info_t *threads = NULL;

/** The global error for the last iteration */
static double global_error;

/*Initialize a barrier that will be used to synchronize the threads.*/ 
pthread_barrier_t barrier; 

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

        /* Initialize global_error to something larger than the
         * tolerance to get the algorithm started */
        global_error = gs_tolerance + 1;

        /* TASK: Initialize global variables here */
	for (int i=0;i<gs_nthreads;i++){
		threads[i].c_row = 0;
        }
	pthread_barrier_init(&barrier,NULL,gs_nthreads); // Initialize the barrier with the total number of threads that will be waiting for the barrier to be released.    
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

        pthread_barrier_destroy(&barrier);
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
                 * to the left before sweeping a row, thread 0 does not need to synchronize with left thread*/
                while ( tid != 0 && threads[tid-1].c_row <= threads[tid].c_row+1) {
                        continue;
                //if the left thread's current row is less than or equal to the current thread's expected next row means the left thread has not finished processing the row that the current thread is waiting for once the left thread moved to the next row the loop exits and alllows current thread to process the row.
                        
                }

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
                threads[tid].c_row++;

                dprintf("%d: row %d done\n", tid, row);
        }
        threads[tid].c_row++; //to ensure that the last thread is not stuck in the while loop waiting for the second last thread to finish processing the last row. because threads are not guaranteed to finish processing the last row at the same time. this ensures threads that finish processing their rows early still update c_row to allow the last thread to finish processing the last row.

        //also the last thread signals others it has finished its last row in order to pass the barrier point and move to next iteration
}

/**
 * Computing routine for each thread
 */
static void *
thread_compute(void *_self)
{
        thread_info_t *self = (thread_info_t *)_self;
        const int tid = self->thread_id;

        /* TASK: Compute bounds for this thread */
        int chunk = gs_size/gs_nthreads;
        int lbound = chunk*tid;
	int rbound = lbound + chunk;
	if (tid == 0)
		lbound++;
	if (tid == gs_nthreads-1)
		rbound--;

        gs_verbose_printf("%i: lbound: %i, rbound: %i\n",
                          tid, lbound, rbound);

        for (int iter = 0;
             iter < gs_iterations && global_error > gs_tolerance;
             iter++) {
                dprintf("%i: Starting iteration %i\n", tid, iter);

                thread_sweep(tid, iter, lbound, rbound);

                /* TASK: Update global error */
                /* Note: The reduction should only be done by one
                 * thread after all threads have updated their local
                 * errors */
                /* Hint: Which thread is guaranteed to complete its
                 * sweep last? */
		if(tid == (gs_nthreads - 1)) {
			// Only the last thread (tid == gs_nthreads - 1) performs the reduction operation and updates the global error, since it's the last to finish.
			global_error = 0;
			for (int i=0;i<gs_nthreads;i++)
				global_error+=threads[i].error;
		}             
                dprintf("%d: iteration %d done\n", tid, iter);

                /* TASK: Iteration barrier because all threads should finish the current iteration before the next one*/
		pthread_barrier_wait(&barrier); //wait for all threads to finish processing its assigned rows
		threads[tid].c_row = 0;
		pthread_barrier_wait(&barrier); // At this point the last thread has updated the global error but wait for all threads to reset the row counter for the next iteration. 
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

        for (int t = 0; t < gs_nthreads; t++) {
                gs_verbose_printf("\tSpawning thread %d\n",t);

                threads[t].thread_id = t;
                err = pthread_create(&threads[t].thread, NULL,
                                     thread_compute, &threads[t]);
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


/*notes for me
1. code comments 
2. a shared status flag array among threads to indicate whether each thread has completed its current row computation. each thread will update its own status flag when completing its task and check the status of its neighboring thread before proceeding?

loop unrolling: we know it is compiler optimization improving the performance by instruction level parallelism
arrange the mmaory accesses in the unrolled loop to maximize data locality and minimize cache misses
*/



/*bonus
1. As more threads are added, the overhead of thread creation, synchronization, and communication can start to outweigh the benefits of parallelism. This is evident from the fact that increasing the number of threads from 4 to 8 or 8 to 16 does not necessarily result in a significant reduction in execution time.

The Gauss-Seidel algorithm itself may not be highly parallelizable due to its iterative nature and dependencies between iterations. Certain parts of the algorithm may not be easily parallelizable, limiting the scalability of the parallel implementation.

2. 
*/
































/*Bonus
1. is performance gain linear with num threads? 
2. What if we wanted to manually specify the memory order (i.e., fence type) for each atomic operation? What is the memory model we are working with? Which operations require fences and which not? You can find more information about the different memory ordering flags available in C here: https://en.cppreference.com/w/c/atomic/memory_orderLinks to an external site.
3. What if we had a TSO memory model and we were not concerned with the C memory model, where would fences be needed then?
4.Suppose we decide to implement the solution differently. We want to use a thread pool, where each thread sits waiting for a task to be assigned. We implement the solution as follows: As soon as a thread finishes with its current task (i.e., computations for a row), it is assigned another row (maybe from another chunk). How would this affect performance? What is the name of the miss that is introduced?*/