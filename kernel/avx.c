#include <assert.h>
#include <immintrin.h>
#include <stdio.h>

#include <sched.h>
#include <sys/time.h>


int main() {

	double rand0 = (double) arc4random();

	if (!__builtin_cpu_supports("avx")) {
		printf("No AVX, skipping test");
		return 0;
	}

	__m256d y0 = _mm256_set_pd(rand0,	1,	-123,	123);
	__m256d y1 = _mm256_set_pd(-1,	rand0,	-121,	121);
	__m256d y2 = _mm256_set_pd(1233.0,	-0.1,	rand0,	1);
	__m256d y3 = _mm256_set_pd(0,		23.0,	-1,	rand0);

	__m256d result_first = _mm256_sub_pd(y0, y1);
	__m256d result_second = _mm256_sub_pd(y2, y3);

	printf("Gonna give the OS opportunity to trash my registers...\n");

	struct timeval start, now;
	int delta;

	(void)gettimeofday(&start, NULL);

	do {
		(void)gettimeofday(&now, NULL);
		delta = now.tv_sec - start.tv_sec;
		sched_yield();
	} while (delta < 3);


	printf("Here's the values I got\n");

	printf("rand0: %lf\n", rand0);
	double* first = (double*)&result_first;
	printf("first %lf\t	%lf\t	%lf\t	%lf\n", first[3], first[2], first[1], first[0]);

	double* second = (double*)&result_second;
	printf("second %lf\t	%lf\t	%lf\t	%lf\n", second[3], second[2], second[1], second[0]);

	printf("Making sure that calculating them by hand gets the same result\n");

#define ALMOST_EQ(A,B) assert((A) - (B) < 0.1)

	ALMOST_EQ(first[3], rand0 - (-1));
	ALMOST_EQ(first[2], 1 - rand0);
	ALMOST_EQ(first[1], -123 - (-121));
	ALMOST_EQ(first[0], 123 - 121);

	ALMOST_EQ(second[3], 1233.0 - (0));
	ALMOST_EQ(second[2], -0.1 - 23.0);
	ALMOST_EQ(second[1], rand0 - (-1));
	ALMOST_EQ(second[0], 1 - rand0);

	printf("Yep!\n");


	return 0;
}
