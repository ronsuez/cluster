#include <stdio.h>
#include <stdlib.h>
 
void Trap(double a, double b, int n, double* global_result_p);
double f(double x);
 
int main(int argc, char* argv[]) {
    double global_result = 0.0;
    double a, b;
    int n;
    printf("Introduce a, b y n \n");
    scanf("%lf %lf %d", &a, &b, &n);
 
    Trap(a, b, n, &global_result);
 
    printf("Con n = %d trapecios, nuestra estimacion\n", n);
    printf("de la integral desde %.1f a %.1f = %.4lf\n", a, b, global_result);
    return 0;
}
 
double f(double x){
    return (double) (x*x)+4;    
}
 
void Trap(double a, double b, int n, double* global_result_p) {
    double h, x, my_result;
    double local_a, local_b;
    int i, local_n;
 
    h = (b-a)/n;
    local_n = n;
    local_a = a;
    local_b = b;
 
 
    my_result = (f(local_a) + f(local_b))/2.0;
 
    for (i = 1; i <= local_n-1; i++) {
        x = local_a + i*h;
        my_result += f(x);
    }
    my_result = my_result*h;
 
    *global_result_p += my_result;
}
