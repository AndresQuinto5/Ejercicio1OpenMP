#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define A 0
#define B 40
#define N 10e6

double f(double x); 
double trapezoides(double a, double b, int n, int thread_count);

int main(int argc, char* argv[]) {
    double integral;
    double a=A, b=B;
    int n=N;
    int thread_count;

    if(argc > 2) {
        a = strtol(argv[1], NULL, 10);
        b = strtol(argv[2], NULL, 10);
    }
    if (argc > 3) {
        thread_count = strtol(argv[3], NULL, 10);
    }
    else {
        thread_count = 1;
    }
    //calculo de tiempo
    double start_time, end_time;
    start_time = omp_get_wtime();
    //---- Aproximacion de la integral
    integral = trapezoides(a,b,n, thread_count);
    end_time = omp_get_wtime();
    printf("Con n = %d trapezoides, nuestra aproximacion \n",n);
    printf("de la integral de %f a %f es = %.10f\n", a,b,integral);
    printf("El tiempo de ejecucion fue %f segundos\n", end_time - start_time);

    return 0;
}

double trapezoides(double a, double b, int n, int thread_count) {
    double h = (b-a)/n;
    double integral = 0.0;

    #pragma omp parallel for num_threads(thread_count) reduction(+:integral)
    for(int k = 0; k <= n; k++) {
        double x = a + k * h;
        double local_int = f(x);

        if(k == 0 || k == n) {
            local_int /= 2.0;
        }

        integral += local_int;
    }

    integral *= h;
    return integral;
}

double f(double x) {
    double return_val;
    return_val = x*x;
    return return_val;
}
