/**
 * SSE matrix multiplication. Bonus assignment.
 *
 *
 * Course: Advanced Computer Architecture, Uppsala University
 * Course Part: Lab assignment 4
 *
 * Author: Andreas Sandberg <andreas.sandberg@it.uu.se>
 *
 * $Id: matmul.c 70 2011-11-22 10:07:10Z ansan501 $
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <smmintrin.h>

#ifndef __SSE4_1__
#error This example requires SSE4.1
#endif

#include "util.h"

/* Size of the matrices to multiply */
#define SIZE 2048

#define SSE_BLOCK_SIZE 4

#ifndef L1_BLOCK_SIZE
#define L1_BLOCK_SIZE 64
#endif

#ifndef L2_BLOCK_SIZE
#define L2_BLOCK_SIZE 1024
#endif

/* A mode that controls how the matrix multiplication is optimized may
 * be specified at compile time. The following modes are defined:
 *
 * MODE_SSE - A simple, non-blocked, implementation of the matrix
 * multiplication.
 *
 * MODE_SSE_BLOCKED - A blocked matrix multiplication with implemented
 * using a 4x4 SSE block.
 *
 * MODE_BLOCKED - Blocked matrix mutliplication using ordinary
 * floating point math.
 */
#define MODE_SSE_BLOCKED 1
#define MODE_SSE 2
#define MODE_BLOCKED 3

#ifndef MODE
#define MODE MODE_SSE
#endif

#define XMM_ALIGNMENT_BYTES 16

static float mat_a[SIZE][SIZE] __attribute__((aligned(XMM_ALIGNMENT_BYTES)));
static float mat_b[SIZE][SIZE] __attribute__((aligned(XMM_ALIGNMENT_BYTES)));
static float mat_c[SIZE][SIZE] __attribute__((aligned(XMM_ALIGNMENT_BYTES)));
static float mat_ref[SIZE][SIZE] __attribute__((aligned(XMM_ALIGNMENT_BYTES)));

// static void
// matmat(size_t n, float mat_c[n][n],
//        const float mat_a[n][n], const float mat_b[n][n])
// {
//     for (int i = 0; i < n; i++)
//         for (int k = 0; k < n; k++)
//             for (int j = 0; j < n; j++)
//                 mat_c[i][j] += mat_a[i][k] * mat_b[k][j];
// }

/**
 * Blocked matrix multiplication, SSE block (4x4 matrix). Implement
 * your solution to the bonus assignment here.
 */

static inline void
matmul_sse_block(int i, int j, int k)
{
        /* BONUS TASK: Implement your SSE 4x4 matrix multiplication
         * block here. */
        /* HINT: You might find at least the following instructions
         * useful:
         *  - _mm_dp_ps
         *  - _MM_TRANSPOSE4_PS
         *
         * HINT: The result of _mm_dp_ps is scalar. The third
         * parameter can be used to restrict to which elements the
         * result is stored, all other elements are set to zero.
         */

        // Load  mat_a and mat_b into SIMD registers
        // __m128 a0 = _mm_loadu_ps(&mat_a[i]+k);
        // __m128 a1 = _mm_loadu_ps(&mat_a[i + 1]+k);
        // __m128 a2 = _mm_loadu_ps(&mat_a[i + 2]+k);
        // __m128 a3 = _mm_loadu_ps(&mat_a[i + 3]+k);

        // __m128 b0 = _mm_loadu_ps(&mat_b[k][j]);
        // __m128 b1 = _mm_loadu_ps(&mat_b[k + 1]+j);
        // __m128 b2 = _mm_loadu_ps(&mat_b[k + 2]+j);
        // __m128 b3 = _mm_loadu_ps(&mat_b[k + 3]+j);

        // // Transpose so that b column will be a register "row"
        // _MM_TRANSPOSE4_PS(b0, b1, b2, b3);

        // // Matrix multiplication for every index of c
        // __m128 c0 = _mm_dp_ps(a0, b0, 0xF1);
        // __m128 c1 = _mm_dp_ps(a0, b1, 0xF1);
        // __m128 c2 = _mm_dp_ps(a0, b2, 0xF1);
        // __m128 c3 = _mm_dp_ps(a0, b3, 0xF1);

        // c0 = _mm_add_ps(c0, _mm_dp_ps(a1, b0, 0xF1));
        // c1 = _mm_add_ps(c1, _mm_dp_ps(a1, b1, 0xF1));
        // c2 = _mm_add_ps(c2, _mm_dp_ps(a1, b2, 0xF1));
        // c3 = _mm_add_ps(c3, _mm_dp_ps(a1, b3, 0xF1));

        // c0 = _mm_add_ps(c0, _mm_dp_ps(a2, b0, 0xF1));
        // c1 = _mm_add_ps(c1, _mm_dp_ps(a2, b1, 0xF1));
        // c2 = _mm_add_ps(c2, _mm_dp_ps(a2, b2, 0xF1));
        // c3 = _mm_add_ps(c3, _mm_dp_ps(a2, b3, 0xF1));

        // c0 = _mm_add_ps(c0, _mm_dp_ps(a3, b0, 0xF1));
        // c1 = _mm_add_ps(c1, _mm_dp_ps(a3, b1, 0xF1));
        // c2 = _mm_add_ps(c2, _mm_dp_ps(a3, b2, 0xF1));
        // c3 = _mm_add_ps(c3, _mm_dp_ps(a3, b3, 0xF1));

        // // Accumulate the results into mat_c
        // __m128 mc0 = _mm_loadu_ps(&mat_c[i][j]);
        // __m128 mc1 = _mm_loadu_ps(&mat_c[i + 1][j]);
        // __m128 mc2 = _mm_loadu_ps(&mat_c[i + 2][j]);
        // __m128 mc3 = _mm_loadu_ps(&mat_c[i + 3][j]);

        // mc0 = _mm_add_ps(mc0, c0);
        // mc1 = _mm_add_ps(mc1, c1);
        // mc2 = _mm_add_ps(mc2, c2);
        // mc3 = _mm_add_ps(mc3, c3);

        // // Store the results back to mat_c
        // _mm_storeu_ps(&mat_c[i][j], mc0);
        // _mm_storeu_ps(&mat_c[i + 1][j], mc1);
        // _mm_storeu_ps(&mat_c[i + 2][j], mc2);
        // _mm_storeu_ps(&mat_c[i + 3][j], mc3);

        ////////////////////////////////////////////////////////////////

        // Load  mat_a and mat_b into SIMD registers
        __m128 a0 = _mm_load_ps(mat_a[i] + k);
        __m128 a1 = _mm_load_ps(mat_a[i + 1] + k);
        __m128 a2 = _mm_load_ps(mat_a[i + 2] + k);
        __m128 a3 = _mm_load_ps(mat_a[i + 3] + k);

        __m128 b0 = _mm_load_ps(mat_b[k] + j);
        __m128 b1 = _mm_load_ps(mat_b[k + 1] + j);
        __m128 b2 = _mm_load_ps(mat_b[k + 2] + j);
        __m128 b3 = _mm_load_ps(mat_b[k + 3] + j);

        // Transpose so that b column will be a register "row"
        _MM_TRANSPOSE4_PS(b0, b1, b2, b3);

        // Matrix multiplication for every index of c
        __m128 c00 = _mm_dp_ps(a0, b0, 0xF1);
        __m128 c01 = _mm_dp_ps(a0, b1, 0xF1);
        __m128 c02 = _mm_dp_ps(a0, b2, 0xF1);
        __m128 c03 = _mm_dp_ps(a0, b3, 0xF1);
        //__m128 c0 = _mm_set_ps(c00, c01, c02, c03);
        _MM_TRANSPOSE4_PS(c00, c01, c02, c03);

        __m128 c10 = _mm_dp_ps(a1, b0, 0xF1);
        __m128 c11 = _mm_dp_ps(a1, b1, 0xF1);
        __m128 c12 = _mm_dp_ps(a1, b2, 0xF1);
        __m128 c13 = _mm_dp_ps(a1, b3, 0xF1);
        //__m128 c1 = _mm_set_ps(c10, c11, c12, c13);
        _MM_TRANSPOSE4_PS(c10, c11, c12, c13);

        __m128 c20 = _mm_dp_ps(a2, b0, 0xF1);
        __m128 c21 = _mm_dp_ps(a2, b1, 0xF1);
        __m128 c22 = _mm_dp_ps(a2, b2, 0xF1);
        __m128 c23 = _mm_dp_ps(a2, b3, 0xF1);
        //__m128 c2 = (c20, c21, c22, c23);
        _MM_TRANSPOSE4_PS(c20, c21, c22, c23);

        __m128 c30 = _mm_dp_ps(a3, b0, 0xF1);
        __m128 c31 = _mm_dp_ps(a3, b1, 0xF1);
        __m128 c32 = _mm_dp_ps(a3, b2, 0xF1);
        __m128 c33 = _mm_dp_ps(a3, b3, 0xF1);
        //__m128 c3 = (c30, c31, c32, c33);
        _MM_TRANSPOSE4_PS(c30, c31, c32, c33);


        _mm_store_ps(mat_c[i] + j, _mm_add_ps(_mm_load_ps(mat_c[i] + j), c00));
        _mm_store_ps(mat_c[i + 1] + j, _mm_add_ps(_mm_load_ps(mat_c[i + 1] + j), c10));
        _mm_store_ps(mat_c[i + 2] + j, _mm_add_ps(_mm_load_ps(mat_c[i + 2] + j), c20));
        _mm_store_ps(mat_c[i + 3] + j, _mm_add_ps( _mm_load_ps(mat_c[i + 3] + j), c30));





        ////////////////////////////////////////////////////////////

        // //Load  mat_a and mat_b into SIMD registers
        // __m128 a0 = _mm_loadu_ps(&mat_a[i]+k);
        // __m128 a1 = _mm_loadu_ps(&mat_a[i + 1]+k);
        // __m128 a2 = _mm_loadu_ps(&mat_a[i + 2]+k);
        // __m128 a3 = _mm_loadu_ps(&mat_a[i + 3]+k);

        // __m128 b0 = _mm_loadu_ps(&mat_b[k]+j);
        // __m128 b1 = _mm_loadu_ps(&mat_b[k + 1]+j);
        // __m128 b2 = _mm_loadu_ps(&mat_b[k + 2]+j);
        // __m128 b3 = _mm_loadu_ps(&mat_b[k + 3]+j);

        // // Transpose so that b column will be a register "row"
        // _MM_TRANSPOSE4_PS(b0, b1, b2, b3);

        // // Matrix multiplication for every index of c
        // __m128 c00 = _mm_dp_ps(a0, b0, 0xF1);
        // __m128 c01 = _mm_dp_ps(a0, b1, 0xF1);
        // __m128 c02 = _mm_dp_ps(a0, b2, 0xF1);
        // __m128 c03 = _mm_dp_ps(a0, b3, 0xF1);
        // // Store the result in c
        // //__m128 c0 = _mm_set_ps(c00, c01, c02, c03);
        // _MM_TRANSPOSE4_PS(c00, c01, c02, c03);

        // __m128 c10 = _mm_dp_ps(a1, b0, 0xF1);
        // __m128 c11 = _mm_dp_ps(a1, b1, 0xF1);
        // __m128 c12 = _mm_dp_ps(a1, b2, 0xF1);
        // __m128 c13 = _mm_dp_ps(a1, b3, 0xF1);
        // //__m128 c1 = _mm_set_ps(c10, c11, c12, c13);
        // _MM_TRANSPOSE4_PS(c10, c11, c12, c13);

        // __m128 c20 = _mm_dp_ps(a2, b0, 0xF1);
        // __m128 c21 = _mm_dp_ps(a2, b1, 0xF1);
        // __m128 c22 = _mm_dp_ps(a2, b2, 0xF1);
        // __m128 c23 = _mm_dp_ps(a2, b3, 0xF1);
        // //__m128 c2 = (c20, c21, c22, c23);
        // _MM_TRANSPOSE4_PS(c20, c21, c22, c23);

        // __m128 c30 = _mm_dp_ps(a3, b0, 0xF1);
        // __m128 c31 = _mm_dp_ps(a3, b1, 0xF1);
        // __m128 c32 = _mm_dp_ps(a3, b2, 0xF1);
        // __m128 c33 = _mm_dp_ps(a3, b3, 0xF1);
        // //__m128 c3 = (c30, c31, c32, c33);
        // _MM_TRANSPOSE4_PS(c30, c31, c32, c33);

        
        // _mm_store_ps(mat_c[i] + j, _mm_add_ps(_mm_load_ps(mat_c[i] + j), c00));
        // _mm_store_ps(mat_c[i + 1] + j, _mm_add_ps(_mm_load_ps(mat_c[i + 1] + j), c10));
        // _mm_store_ps(mat_c[i + 2] + j, _mm_add_ps(_mm_load_ps(mat_c[i + 2] + j), c20));
        // _mm_store_ps(mat_c[i + 3] + j, _mm_add_ps( _mm_load_ps(mat_c[i + 3] + j), c30));

        // // Accumulate the results into mat_c
        // __m128 mc0 = _mm_loadu_ps(&mat_c[i][j]);
        // __m128 mc1 = _mm_loadu_ps(&mat_c[i + 1][j]);
        // __m128 mc2 = _mm_loadu_ps(&mat_c[i + 2][j]);
        // __m128 mc3 = _mm_loadu_ps(&mat_c[i + 3][j]);

        // mc0 = _mm_add_ps(mc0, c0);
        // mc1 = _mm_add_ps(mc1, c1);
        // mc2 = _mm_add_ps(mc2, c2);
        // mc3 = _mm_add_ps(mc3, c3);

        // // Store the results back to mat_c
        // _mm_storeu_ps(&mat_c[i][j], mc0);
        // _mm_storeu_ps(&mat_c[i + 1][j], mc1);
        // _mm_storeu_ps(&mat_c[i + 2][j], mc2);
        // _mm_storeu_ps(&mat_c[i + 3][j], mc3);

        // _mm_storeu_ps(&mat_c[i][j], c0);
        // _mm_storeu_ps(&mat_c[i + 1][j], c1);
        // _mm_storeu_ps(&mat_c[i + 2][j], c2);
        // _mm_storeu_ps(&mat_c[i + 3][j], c3);


        // _mm_storeu_ps(&mat_c[i][j], _mm_add_ps(_mm_load_ps(&mat_c[i][j]), c0));
        // _mm_storeu_ps(&mat_c[i + 1][j], _mm_add_ps(_mm_load_ps(&mat_c[i + 1][j]), c1));
        // _mm_storeu_ps(&mat_c[i + 2][j], _mm_add_ps(_mm_load_ps(&mat_c[i + 2][j]), c2));
        // _mm_storeu_ps(&mat_c[i + 3][j], _mm_add_ps(_mm_load_ps(&mat_c[i + 3][j]), c3));
}

/**
 * Blocked matrix multiplication, SSE block (4x4 matrix) implemented
 * using ordinary floating point math.
 */
static inline void
matmul_block(int i, int j, int k)
{
        mat_c[i][j] +=
            mat_a[i][k] * mat_b[k][j] + mat_a[i][k + 1] * mat_b[k + 1][j] + mat_a[i][k + 2] * mat_b[k + 2][j] + mat_a[i][k + 3] * mat_b[k + 3][j];

        mat_c[i][j + 1] +=
            mat_a[i][k] * mat_b[k][j + 1] + mat_a[i][k + 1] * mat_b[k + 1][j + 1] + mat_a[i][k + 2] * mat_b[k + 2][j + 1] + mat_a[i][k + 3] * mat_b[k + 3][j + 1];

        mat_c[i][j + 2] +=
            mat_a[i][k] * mat_b[k][j + 2] + mat_a[i][k + 1] * mat_b[k + 1][j + 2] + mat_a[i][k + 2] * mat_b[k + 2][j + 2] + mat_a[i][k + 3] * mat_b[k + 3][j + 2];

        mat_c[i][j + 3] +=
            mat_a[i][k] * mat_b[k][j + 3] + mat_a[i][k + 1] * mat_b[k + 1][j + 3] + mat_a[i][k + 2] * mat_b[k + 2][j + 3] + mat_a[i][k + 3] * mat_b[k + 3][j + 3];

        mat_c[i + 1][j] +=
            mat_a[i + 1][k] * mat_b[k][j] + mat_a[i + 1][k + 1] * mat_b[k + 1][j] + mat_a[i + 1][k + 2] * mat_b[k + 2][j] + mat_a[i + 1][k + 3] * mat_b[k + 3][j];

        mat_c[i + 1][j + 1] +=
            mat_a[i + 1][k] * mat_b[k][j + 1] + mat_a[i + 1][k + 1] * mat_b[k + 1][j + 1] + mat_a[i + 1][k + 2] * mat_b[k + 2][j + 1] + mat_a[i + 1][k + 3] * mat_b[k + 3][j + 1];

        mat_c[i + 1][j + 2] +=
            mat_a[i + 1][k] * mat_b[k][j + 2] + mat_a[i + 1][k + 1] * mat_b[k + 1][j + 2] + mat_a[i + 1][k + 2] * mat_b[k + 2][j + 2] + mat_a[i + 1][k + 3] * mat_b[k + 3][j + 2];

        mat_c[i + 1][j + 3] +=
            mat_a[i + 1][k] * mat_b[k][j + 3] + mat_a[i + 1][k + 1] * mat_b[k + 1][j + 3] + mat_a[i + 1][k + 2] * mat_b[k + 2][j + 3] + mat_a[i + 1][k + 3] * mat_b[k + 3][j + 3];

        mat_c[i + 2][j] +=
            mat_a[i + 2][k] * mat_b[k][j] + mat_a[i + 2][k + 1] * mat_b[k + 1][j] + mat_a[i + 2][k + 2] * mat_b[k + 2][j] + mat_a[i + 2][k + 3] * mat_b[k + 3][j];

        mat_c[i + 2][j + 1] +=
            mat_a[i + 2][k] * mat_b[k][j + 1] + mat_a[i + 2][k + 1] * mat_b[k + 1][j + 1] + mat_a[i + 2][k + 2] * mat_b[k + 2][j + 1] + mat_a[i + 2][k + 3] * mat_b[k + 3][j + 1];

        mat_c[i + 2][j + 2] +=
            mat_a[i + 2][k] * mat_b[k][j + 2] + mat_a[i + 2][k + 1] * mat_b[k + 1][j + 2] + mat_a[i + 2][k + 2] * mat_b[k + 2][j + 2] + mat_a[i + 2][k + 3] * mat_b[k + 3][j + 2];

        mat_c[i + 2][j + 3] +=
            mat_a[i + 2][k] * mat_b[k][j + 3] + mat_a[i + 2][k + 1] * mat_b[k + 1][j + 3] + mat_a[i + 2][k + 2] * mat_b[k + 2][j + 3] + mat_a[i + 2][k + 3] * mat_b[k + 3][j + 3];

        mat_c[i + 3][j] +=
            mat_a[i + 3][k] * mat_b[k][j] + mat_a[i + 3][k + 1] * mat_b[k + 1][j] + mat_a[i + 3][k + 2] * mat_b[k + 2][j] + mat_a[i + 3][k + 3] * mat_b[k + 3][j];

        mat_c[i + 3][j + 1] +=
            mat_a[i + 3][k] * mat_b[k][j + 1] + mat_a[i + 3][k + 1] * mat_b[k + 1][j + 1] + mat_a[i + 3][k + 2] * mat_b[k + 2][j + 1] + mat_a[i + 3][k + 3] * mat_b[k + 3][j + 1];

        mat_c[i + 3][j + 2] +=
            mat_a[i + 3][k] * mat_b[k][j + 2] + mat_a[i + 3][k + 1] * mat_b[k + 1][j + 2] + mat_a[i + 3][k + 2] * mat_b[k + 2][j + 2] + mat_a[i + 3][k + 3] * mat_b[k + 3][j + 2];

        mat_c[i + 3][j + 3] +=
            mat_a[i + 3][k] * mat_b[k][j + 3] + mat_a[i + 3][k + 1] * mat_b[k + 1][j + 3] + mat_a[i + 3][k + 2] * mat_b[k + 2][j + 3] + mat_a[i + 3][k + 3] * mat_b[k + 3][j + 3];

        /*
         * The code in this function can alternatively be expressed using macros:
         */
        /*
        #define BLOCK_SUB(n, m, l) ( mat_a[i + n][k + l] * mat_b[k + l][j + m] )

        #define BLOCK_CELL(n, m) { \
                mat_c[i + n][j + m] += \
                        BLOCK_SUB(n, m, 0) + \
                        BLOCK_SUB(n, m, 1) + \
                        BLOCK_SUB(n, m, 2) + \
                        BLOCK_SUB(n, m, 3) ; \
        }

        #define BLOCK_ROW(n) { \
                BLOCK_CELL(n, 0); \
                BLOCK_CELL(n, 1); \
                BLOCK_CELL(n, 2); \
                BLOCK_CELL(n, 3); \
        }

                BLOCK_ROW(0);
                BLOCK_ROW(1);
                BLOCK_ROW(2);
                BLOCK_ROW(3);
        */
}

#if MODE == MODE_SSE_BLOCKED || MODE == MODE_BLOCKED
/**
 * Blocked matrix multiplication, L1 block.
 */
static inline void
matmul_block_l1(int i, int j, int k)
{
        int ii, jj, kk;

        for (ii = i; ii < i + L1_BLOCK_SIZE; ii += SSE_BLOCK_SIZE)
                for (kk = k; kk < k + L1_BLOCK_SIZE; kk += SSE_BLOCK_SIZE)
                        for (jj = j; jj < j + L1_BLOCK_SIZE; jj += SSE_BLOCK_SIZE)
                        {
#if MODE == MODE_SSE_BLOCKED
                                matmul_sse_block(ii, jj, kk);
#elif MODE == MODE_BLOCKED
                                matmul_block(ii, jj, kk);
#endif
                        }
}

/**
 * Blocked matrix multiplication, L2 block.
 */
static inline void
matmul_block_l2(int i, int j, int k)
{
        int ii, jj, kk;

        for (ii = i; ii < i + L2_BLOCK_SIZE; ii += L1_BLOCK_SIZE)
                for (kk = k; kk < k + L2_BLOCK_SIZE; kk += L1_BLOCK_SIZE)
                        for (jj = j; jj < j + L2_BLOCK_SIZE; jj += L1_BLOCK_SIZE)
                                matmul_block_l1(ii, jj, kk);
}

/**
 * Blocked matrix multiplication, entry function for multiplying two
 * matrices.
 */
static void
matmul_sse()
{
        int i, j, k;

        for (i = 0; i < SIZE; i += L2_BLOCK_SIZE)
                for (k = 0; k < SIZE; k += L2_BLOCK_SIZE)
                        for (j = 0; j < SIZE; j += L2_BLOCK_SIZE)
                                matmul_block_l2(i, j, k);
}

#elif MODE == MODE_SSE

/**
 * Matrix multiplication. This is the procedure you should try to
 * optimize.
 */
static void
matmul_sse()
{
        int i, j, k;

        /* Assume that the data size is an even multiple of the 128 bit
         * SSE vectors (i.e. 4 floats) */
        assert(!(SIZE & 0x3));

        /* TASK: Implement your simple matrix multiplication using SSE
         * here. (Multiply mat_a and mat_b into mat_c.)
         */

        /*
        static void
        matmat(size_t n, float mat_c[n][n], const float mat_a[n][n], const float mat_b[n][n])
{
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
            for (int j = 0; j < n; j++)
                mat_c[i][j] += mat_a[i][k] * mat_b[k][j];
}*/
        for (i = 0; i < SIZE; i++)
        {
                for (k = 0; k < SIZE; k++)
                {
                        __m128 a = _mm_set1_ps(mat_a[i][k]);
                        for (j = 0; j < SIZE; j += 4)
                        {
                                __m128 col = _mm_load_ps(mat_b[k] + j);
                                _mm_store_ps(mat_c[i] + j, _mm_add_ps(_mm_load_ps(mat_c[i] + j), _mm_mul_ps(a, col)));
                        }
                }
        }
}

#else

#error Invalid mode

#endif

/**
 * Reference implementation of the matrix multiply algorithm. Used to
 * verify the answer from matmul_opt. Do NOT change this function.
 */
static void
matmul_ref()
{
        int i, j, k;

        for (i = 0; i < SIZE; i++)
        {
                for (k = 0; k < SIZE; k++)
                {
                        for (j = 0; j < SIZE; j++)
                        {
                                mat_ref[i][j] += mat_a[i][k] * mat_b[k][j];
                        }
                }
        }
}

/**
 * Function used to verify the result. No need to change this one.
 */
static int
verify_result()
{
        float e_sum;
        int i, j;

        e_sum = 0;
        for (i = 0; i < SIZE; i++)
        {
                for (j = 0; j < SIZE; j++)
                {
                        e_sum += mat_c[i][j] < mat_ref[i][j] ? mat_ref[i][j] - mat_c[i][j] : mat_c[i][j] - mat_ref[i][j];
                }
        }

        printf("e_sum: %.e\n", e_sum);

        return e_sum < 1E-6;
}

/**
 * Initialize mat_a and mat_b with "random" data. Write to every
 * element in mat_c to make sure that the kernel allocates physical
 * memory to every page in the matrix before we start doing
 * benchmarking.
 */
static void
init_matrices()
{
        int i, j;

        for (i = 0; i < SIZE; i++)
        {
                for (j = 0; j < SIZE; j++)
                {
                        mat_a[i][j] = ((i + j) & 0x0F) * 0x1P-4F;
                        mat_b[i][j] = (((i << 1) + (j >> 1)) & 0x0F) * 0x1P-4F;
                }
        }

        memset(mat_c, 0, sizeof(mat_c));
        memset(mat_ref, 0, sizeof(mat_ref));
}

static int
run_multiply()
{
        struct timespec ts_start, ts_stop;
        double runtime_ref, runtime_sse;

        printf("Starting optimized run...\n");
        util_monotonic_time(&ts_start);
        /* mat_c = mat_a * mat_b */
        matmul_sse();
        util_monotonic_time(&ts_stop);
        runtime_sse = util_time_diff(&ts_start, &ts_stop);
        printf("Optimized run completed in %.2f s\n",
               runtime_sse);

        printf("Starting reference run...\n");
        util_monotonic_time(&ts_start);
        matmul_ref();
        util_monotonic_time(&ts_stop);
        runtime_ref = util_time_diff(&ts_start, &ts_stop);
        printf("Reference run completed in %.2f s\n",
               runtime_ref);

        printf("Speedup: %.2f\n",
               runtime_ref / runtime_sse);

        if (verify_result())
        {
                printf("OK\n");
                return 0;
        }
        else
        {
                printf("MISMATCH\n");
                return 1;
        }
}

int main(int argc, char *argv[])
{
        /* Initialize the matrices with some "random" data. */
        init_matrices();

        int rc = run_multiply();
        if (rc)
                return 1;

        return 0;
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 8
 * indent-tabs-mode: nil
 * c-file-style: "linux"
 * compile-command: "make -k"
 * End:
 */
