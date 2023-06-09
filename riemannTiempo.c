/*----------------------------------------------
 * riemann.c - calculo de area bajo la curva
 *----------------------------------------------
 * Sumas de Riemann para calcular la integral f(x)
 *
 * Date:  2021-09-22
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define A 0
#define B 40
#define N 10e6

double f(double x);     // La funcion a integrar
double trapezoides(double a, double b, int n);

int main(int argc, char* argv[]) {
  double integral;
  double a = A, b = B;
  int n = N;
  double h;

  if (argc > 2) {
    a = strtol(argv[1], NULL, 10);
    b = strtol(argv[2], NULL, 10);
  }

  //---- Aproximacion de la integral
  h = (b - a) / n;
  
  double start_time = omp_get_wtime(); // Iniciar el tiempo de ejecución

  integral = trapezoides(a, b, n);

  double end_time = omp_get_wtime(); // Finalizar el tiempo de ejecución
  double execution_time = end_time - start_time;

  printf("Con n = %d trapezoides, nuestra aproximacion \n", n);
  printf("de la integral de %f a %f es = %.10f\n", a, b, integral);
  printf("Tiempo de ejecución: %f segundos\n", execution_time);

  return 0;
}/*main*/

//------------------------------------------
// trapezoides
//
// Estimar la integral mediante sumas de Riemann
// Input: a,b,n,h
// Output: integral
//------------------------------------------
double trapezoides(double a, double b, int n) {
  double integral, h;
  int k;

  //---- Ancho de cada trapezoide
  h = (b - a) / n;
  //---- Valor inicial de la integral (valores extremos)
  integral = (f(a) + f(b)) / 2.0;

  for (k = 1; k <= n - 1; k++) {
    integral += f(a + k * h);
  }
  integral = integral * h;

  return integral;
}/*trapezoides*/

//------------------------------------------
// f
//
// Funcion a ser integrada
// Input: x
//------------------------------------------
double f(double x) {
  double return_val;

  return_val = x * x;

  return return_val;
}/*f*/
