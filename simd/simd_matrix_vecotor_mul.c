#include "headers.h"

#ifdef __ARM_NEON
#include <arm_neon.h>

#elif __X86_SSE
#include <immintrin.h>
#endif

#define NROUNDS 1

float test_matrix[1000*1000];
float test_vector[1000];
float new_vec[1000];

#ifdef __X86_SSE
inline __m256 mul8( const float* p1, const float* p2, unsigned int offsetRegs )
{
    int lanes = offsetRegs * 8;
    const __m256 a = _mm256_loadu_ps( p1 + lanes );
    const __m256 b = _mm256_loadu_ps( p2 + lanes );
    return _mm256_mul_ps( a, b );
}
inline __m256 fma8( __m256 acc, const float* p1, const float* p2, unsigned int offsetRegs )
{
    unsigned int lanes = offsetRegs * 8;
    const __m256 a = _mm256_loadu_ps( p1 + lanes );
    const __m256 b = _mm256_loadu_ps( p2 + lanes );
    return _mm256_fmadd_ps( a, b, acc );
}
// x = ( x7, x6, x5, x4, x3, x2, x1, x0 )
inline float sum8(__m256 x) {
    // hiQuad = ( x7, x6, x5, x4 )
    const __m128 hiQuad = _mm256_extractf128_ps(x, 1);
    // loQuad = ( x3, x2, x1, x0 )
    const __m128 loQuad = _mm256_castps256_ps128(x);
    // sumQuad = ( x3 + x7, x2 + x6, x1 + x5, x0 + x4 )
    const __m128 sumQuad = _mm_add_ps(loQuad, hiQuad);
    // loDual = ( -, -, x1 + x5, x0 + x4 )
    const __m128 loDual = sumQuad;
    // hiDual = ( -, -, x3 + x7, x2 + x6 )
    const __m128 hiDual = _mm_movehl_ps(sumQuad, sumQuad);
    // sumDual = ( -, -, x1 + x3 + x5 + x7, x0 + x2 + x4 + x6 )
    const __m128 sumDual = _mm_add_ps(loDual, hiDual);
    // lo = ( -, -, -, x0 + x2 + x4 + x6 )
    const __m128 lo = sumDual;
    // hi = ( -, -, -, x1 + x3 + x5 + x7 )
    const __m128 hi = _mm_shuffle_ps(sumDual, sumDual, 0x1);
    // sum = ( -, -, -, x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 )
    const __m128 sum = _mm_add_ss(lo, hi);
    return _mm_cvtss_f32(sum);
}
void matrix_vec_mult_avx(float *mat, uint32_t dim_size, float *vec, float *new_vec) {
    float *row = mat, *matend = mat + dim_size*dim_size;
    __m256 dot0, dot1, dot2, dot3;
    int k = 0;
    while(row < matend) {
        float *col = row;
        float *vecptr = vec;

        dot0 = mul8(col,vecptr,0);
        vecptr += 8;
        col += 8;
        while(vecptr < vec + dim_size) {
            dot0 = fma8(dot0,col,vecptr,0);
            vecptr += 8;
            col += 8;
        }
        new_vec[k] = sum8(dot0);
        k++;
        row += dim_size; // go to next row of matrix
    }
}

#elif __ARM_NEON
void matrix_multiply_neon(float32_t  *A, float32_t  *B, float32_t *C, uint32_t n, uint32_t m, uint32_t k) {
        int A_idx;
        int B_idx;
        int C_idx;
        
        // these are the columns of a 4x4 sub matrix of A
        float32x4_t A0;
        float32x4_t A1;
        float32x4_t A2;
        float32x4_t A3;
        
        // these are the columns of a 4x1 sub matrix of B
        float32x4_t B0;
        
        // these are the columns of a 4x1 sub matrix of C
        float32x4_t C0;
        
        for (int i_idx=0; i_idx<n; i_idx+=4) {
                for (int j_idx=0; j_idx<m; j_idx+=4) {
                        // Zero accumulators before matrix op
                        C0 = vmovq_n_f32(0);
                        for (int k_idx=0; k_idx<k; k_idx+=4) {
                                // Compute base index to 4x4 block
                                A_idx = i_idx + n*k_idx;
                                B_idx = k*j_idx + k_idx;
                                
                                // Load most current A values in row 
                                A0 = vld1q_f32(A+A_idx);
                                A1 = vld1q_f32(A+A_idx+n);
                                A2 = vld1q_f32(A+A_idx+2*n);
                                A3 = vld1q_f32(A+A_idx+3*n);
                                
                                // Multiply accumulate in 4x1 blocks, i.e. each column in C
                                B0 = vld1q_f32(B+B_idx);
                                C0 = vfmaq_laneq_f32(C0, A0, B0, 0);
                                C0 = vfmaq_laneq_f32(C0, A1, B0, 1);
                                C0 = vfmaq_laneq_f32(C0, A2, B0, 2);
                                C0 = vfmaq_laneq_f32(C0, A3, B0, 3);
                                
                        }
                        // Compute base index for stores
                        C_idx = n*j_idx + i_idx;
                        vst1q_f32(C+C_idx, C0);
                }
        }
}
#endif

void random_vector_matrix(int N){
	for(int i=0; i<N; i++) {
		test_vector[i] = rand();
		for(int j=0; j<N; j++){
			test_matrix[i+j] = rand();
		}
	}
}

void print_vector_matrix(int N){
	printf("Test Vector:\n");
	for (int q = 0; q < N; q++) { printf("%d, %f\n", q, test_vector[q]); }
	printf("Test Matrix:\n");
	for (int r = 0; r < N; r++) {
		for (int s = 0; s < N; s++) {
			printf("%f ", test_matrix[r*N + s]);
		}
		printf("\n");
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	//declaration
	time_t t;
	//srand((unsigned) time(&t));
 	clock_t start, end;
	double cpu_time_used;
	int N;

	//initalizations
	N = atoi(argv[1]);
	random_vector_matrix(N);

	//printing of matrix and vector
    	//print_vector_matrix(N);

	//benchmarking
	//x86 or arm simd
	cpu_time_used = 0.;
#ifdef __X86_SSE
	for(int x = 0; x < NROUNDS; x++){
		start = clock();
		matrix_vec_mult_avx(test_matrix, N, test_vector, new_vec);
		end = clock();
		cpu_time_used += ((double) (end - start))/CLOCKS_PER_SEC*1000;
	}
        printf("%fms x86\n", cpu_time_used/NROUNDS);
#elif __ARM_NEON
	for(int x = 0; x < NROUNDS; x++){
		start = clock();
		matrix_multiply_neon(test_matrix, test_vector, new_vec, N, N, N);
		end = clock();
		cpu_time_used += ((double) (end - start))/CLOCKS_PER_SEC*1000000;
	}
        printf("%fµs arm\n", cpu_time_used/NROUNDS);
#endif
	return 0;
}
