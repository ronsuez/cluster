#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
double f(double x){
    return (double) (x*x)+4;    
}
 
double ReglaTrapecio(double a, double b, int n, double h)
{
    int i;
    double mi_resultado, x;
    mi_resultado = (f(a) + f(b))/2.0;
 
    for (i = 1; i <= n-1; i++) {
        x = a + i*h;
        mi_resultado += f(x);
    }
    mi_resultado = mi_resultado*h;
 
    return mi_resultado;
}
 
int main(int argc, char **argv) 
{
    int size, my_rank;
    MPI_Status status;
 
    MPI_Init(&argc, &argv);
     
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
 
    int proc;
    double a,b, local_b, h, local_integral, total_integral, h_p;
    int n , local_n ,  n_p;
    double a_p, b_p;
 
     
    if( my_rank == 0)
    {
        do{
            printf("Introduce a, b y n que sea divisible por cantida de procesos (%d) \n",size);
            scanf("%lf %lf %d", &a, &b, &n);
        }while(n%size!=0);
         
        h = (b-a)/n;
        local_n = n/size;
        for (proc = 1 ; proc < size ; proc++)
        {           
            a_p = a + proc*local_n*h;
            b_p = a_p + local_n*h;
            MPI_Send(&a_p, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
            MPI_Send(&b_p, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
            MPI_Send(&local_n, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
            MPI_Send(&h, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
            printf("Soy el proceso %d y he enviado a P%d : a= %.0f, b= %.0f, n= %d, h = %.3f\n\n",my_rank,proc,a_p, b_p, local_n,h);
        }
 
        local_b = a + local_n*h;
        local_integral = ReglaTrapecio(a, local_b, local_n, h); 
        total_integral += local_integral;   
        printf("Soy el proceso %d y tengo integral de %.0f a %.0f = %.3f\n\n",my_rank,a, local_b, local_integral);
 
        for (proc = 1; proc < size ; proc++) 
        {
            MPI_Recv(&local_integral, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);   
            total_integral += local_integral;
        }
 
        printf("\nResultado de Integral es: %lf\n\n\n",total_integral);
     
    }
    if( my_rank != 0)
    {
        MPI_Recv(&a, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&b, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&n_p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&h_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Soy el proceso %d y he recibido a= %.0f, b= %.0f, n= %d, h = %.3f\n\n",my_rank,a, b, n_p,h_p);
        local_integral= ReglaTrapecio(a, b, n_p, h_p);
        printf("Soy el proceso %d y tengo integral de %.0f a %.0f = %.3f\n\n",my_rank,a, b, local_integral);
        MPI_Send(&local_integral, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
         
    }   
 
    MPI_Finalize();
 
}
