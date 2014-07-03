#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
 
#define M 1000 /*Filas*/
#define N 1000 /*Columnas*/
 
 
int main(int argc, char **argv) 
{
	static double
	        a[N],				/*Vector A*/
			b[M+(N*M)],			/*Matriz B*/
			c[M*N],				/*Vector Fila resultante C*/			
			vaux[M*N];
			
	int 	size, 				/*Cantidad de procesos*/
			my_rank,			/*Numero de proceso*/
			num_procesos,		/*Cantidad de procesos 'workers'*/
			col_proc,		
			m ,n,				/*Fila y columna*/
			i,j,k,aux,				/*Variables de control*/
            hilos, hilos_new;      
    MPI_Status status;
 
    MPI_Init(&argc, &argv);
     
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    num_procesos = size-1; 
    hilos = 10;
    if( my_rank == 0)
    {
		do{
			printf("Procesos a Ejecutar %d\n",num_procesos);
			printf("Indica filas y columnas\n");
			scanf("%d %d",&m,&n);
			printf("Filas: %d - Columnas %d\n",m,n);

            col_proc = n / num_procesos;
         }while (n % num_procesos != 0);
         
         		printf("Vector [");
        for (i = 0 ; i < m; i++)
        {
            a[i] = i+1; 
            printf(" %.0lf ",a[i]);
        }
        printf("]\n");
        for ( i = 0 ; i < n ; i++)
            for (j = 0; j < m ; j++)
            {
                b[j + (i*m)] = i*(j+1);
              //  printf("mat[%d]= %.3lf\n", j+(i*m),b[j+(i*m)]);

            }

        printf("Matriz\n");
		for ( i = 0 ; i < m ; i ++)
		{
			printf("[");
			
			for ( k = 0 ; k < n ; k++)
			{
				//printf("- %d",k);
				printf(" %.0lf ",b[i + (k*m)]);
			}
			printf("]\n");
		}
	/*Enviar datos de entrada a los procesos*/
		
		for (i = 1 ; i <= num_procesos ; i++)
		{			
			//printf("Soy el proceso %d, estoy enviando a P%d datos para su tarea\n",my_rank, i);
			MPI_Send(&m, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&a, m, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			MPI_Send(&b, (m*n), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			MPI_Send(&col_proc, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
        for (i = 1 ; i <=num_procesos ; i++)
		{
			MPI_Recv(&aux, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&vaux, aux, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			
			
			int  min, max;
			min = (i - 1) * col_proc;
			max = i * col_proc;
			for ( j = min ; j < max ; j++)
			{
				if( min == 0)
					c[j] = vaux[j];
				else
					c[j] = vaux[j - min];
			}
			//printf("Soy el proceso %d, estoy recibiendo de P%d resultados\n",my_rank, i);
		}
		
		printf("\n\nVector Resultado [");
		
		for (i= 0 ; i < n ; i++)
		{
			printf("%.0lf ",c[i]);
		}
		printf("]\n");
    }
     
    if( my_rank != 0)
    {	
		MPI_Recv(&m, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&a, m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&b, (m*n), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&col_proc, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		//printf("Soy el proceso %d, estoy recibiendo de P%d \n",my_rank, 0);
		int i, j;
		int   h_min, h_max,min, max,cont, total, col_hilos,colrest_hilos;
        int iam = 0 , np = 1;
		min = (my_rank - 1) * col_proc;
		max = my_rank * col_proc;
		total = max - min;
		
		col_hilos = col_proc / hilos;
		colrest_hilos = col_proc % hilos;
		
		if(col_proc < hilos)
			hilos = col_proc;

		printf("Soy el proceso %d, ini %d fin %d\n",my_rank, min, max);
		 double * s = malloc(sizeof(double)*100000);
		cont = 0;		
		
		  #pragma omp parallel default(shared) private(iam, np)
			{
				np = omp_get_num_threads();
				iam = omp_get_thread_num();
				printf("Hello from thread %d out of %d from process %d\n", iam, np, my_rank);
					for (i = min ; i < max ; i++)
					{
						for( j = 0 ; j < m; j++ )
						{
							s[cont] += a[j] * b[ (i*m) + j ];
									
						}
						cont++;
					}
					cont++;
          
			  
			}
		
		MPI_Send(&total, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(s, total, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        free(s);
    }   
     

    MPI_Finalize();
 
}
