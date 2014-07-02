#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

double f(double x){
    return (double) (x*x)+4;  
}

void thread_trapecio(double a, double b, int n, int my_rank, int thread_count, double* global_result_h) {
    double h, x, my_result;
    double local_a, local_b;
    int i, local_n;

    h = (b-a)/n;
    local_n = n/thread_count;
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;


    my_result = (f(local_a) + f(local_b))/2.0;

    for (i = 1; i <= local_n-1; i++) {
        x = local_a + i*h;
        my_result += f(x);
    }
    my_result = my_result*h;

#pragma omp critical
    *global_result_h += my_result;
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
    int size, my_rank, namelen;
    MPI_Status status;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(processor_name, &namelen);
    int thread_count;
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
        do{
		printf("Indique cantidad de hilos que divisible por n = %d ... Es Decir, hilos mod n = 0\n",local_n);
        	scanf("%d",&thread_count);
	}while ( n % thread_count != 0);
        for (proc = 1 ; proc < size ; proc++)
        { 
            a_p = a + proc*local_n*h;
            b_p = a_p + local_n*h;
            MPI_Send(&a_p, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
            MPI_Send(&b_p, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
            MPI_Send(&local_n, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
            MPI_Send(&thread_count, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
            printf("\nSoy el proceso %d y he enviado a P%d : a= %.0f, b= %.0f, n= %d, h = %.3f\n\n",my_rank,proc,a_p, b_p, local_n,h);
        }

        local_b = a + local_n*h;
        local_integral = ReglaTrapecio(a, local_b, local_n, h); 
        total_integral += local_integral; 
        printf("Soy el proceso %d y tengo integral de %.0f a %.0f = %.3f\n\n",my_rank,a, local_b, local_integral);

        for (proc = 1; proc < size; proc++) 
        {
            MPI_Recv(&local_integral, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
            total_integral += local_integral;
        }

        printf("\nResultado de Integral es: %lf\n\n\n",total_integral);
    }
    if( my_rank != 0)
    {
        double integral_hilo = 0.0 ;
        int iam = 0 , np = 1;
        MPI_Recv(&a, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&b, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&n_p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&thread_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Soy el proceso %d y he recibido a= %.0f, b= %.0f, n= %d\n\n",my_rank,a, b, n_p);


#pragma omp parallel default(shared) private(iam ,np) num_threads(thread_count)
        {
            np=omp_get_num_threads();
            iam=omp_get_thread_num();
            thread_trapecio(a, b , n_p, iam, np, &integral_hilo) ;
        }
        printf("Soy el proceso %d y tengo integral de %.0f a %.0f = %.3f\n\n",my_rank,a, b, integral_hilo);
        MPI_Send(&integral_hilo, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

    } 

    MPI_Finalize();

}

