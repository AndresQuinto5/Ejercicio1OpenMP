#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define A 0
#define B 40
#define N 10e6
#define MAX_THREADS 128

double f(double x);
double trapezoides(double a, double b, int n, int thread_count);

int main(int argc, char* argv[]) {
    double integral;
    double a = A, b = B;
    int n = N;
    int thread_count;

    if (argc > 2) {
        a = strtol(argv[1], NULL, 10);
        b = strtol(argv[2], NULL, 10);
    }
    if (argc > 3) {
        thread_count = strtol(argv[3], NULL, 10);
    } else {
        thread_count = 1;
    }
    //calculo de tiempo
    double start_time, end_time;
    start_time = omp_get_wtime();
    //---- Aproximacion de la integral
    integral = trapezoides(a, b, n, thread_count);
    end_time = omp_get_wtime();
    printf("Con n = %d trapezoides, nuestra aproximacion \n", n);
    printf("de la integral de %f a %f es = %.10f\n", a, b, integral);
    printf("El tiempo de ejecucion fue %f segundos\n", end_time - start_time);

    return 0;
}

double trapezoides(double a, double b, int n, int thread_count) {
    double integral = 0.0;
    double h = (b - a) / n;
    int k;

    #pragma omp parallel num_threads(thread_count) private(k) reduction(+:integral)
    {
        double x;
        double local_a, local_b;
        double local_int = 0.0;

        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        int n_local = n / num_threads;
        local_a = a + thread_id * n_local * h;
        local_b = local_a + n_local * h;

        local_int += (f(local_a) + f(local_b)) / 2.0;

        for (k = 1; k <= n_local - 1; k++) {
            x = local_a + k * h;
            local_int += f(x);
        }

        local_int *= h;

        printf("El hilo %d ha calculado la integral local [%f,%f] = %f\n", thread_id, local_a, local_b, local_int);

        integral += local_int;
    }

    return integral;
}

double f(double x) {
    double return_val;
    return_val = x * x;
    return return_val;
}
