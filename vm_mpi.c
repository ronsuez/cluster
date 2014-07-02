#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <time.h>  

#define M 100 /*Filas*/
#define N 100 /*Columnas*/


int main(int argc, char **argv) 
{
	double	a[N],				/*Vector A*/
			b[M+(N*M)],			/*Matriz B*/
			c[N];				/*Vector Fila resultante C*/			
	
	double	s;		
	
	int 	size, 				/*Cantidad de procesos*/
			my_rank,			/*Numero de proceso*/
			num_procesos,		/*Cantidad de procesos 'workers'*/
			col_proc,			/*columnas por proceso*/
			col_rest,			/*Columnas restantes*/
			m ,n,				/*Fila y columna*/
			i,j,k;				/*Variables de control*/
	MPI_Status status;

	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	num_procesos = size-1;
	
	if( my_rank == 0)
	{
		//printf("Indicar Matriz\n");
		/*while (m>M)
		{
			printf("Numero de Filas: ");
			scanf("%d", &m);
			printf("Numero de Columnas: %d\n\n", num_procesos);

			if((m>M)){
				printf("Error, tamano demasiado grande.\n");
			}

		}*/
		
		printf ("Soy el proceso %d",my_rank);
		m = 3;
		
		n = num_procesos;

		/*for(i=0; i<m; i++){
			printf("vec[%d]= ", i);
			scanf("%lf", &a[i]);
		}

		for(i=0; i<n; i++){
			for(j=0; j<m; j++){
				printf("\nmat[%d][%d]= ",j, i);
				scanf("%lf", &b[j + (i*m)]);
			}
		}*/
        for (i = 0 ; i < m; i++)
        {
            a[i] = i+1; 
            printf("vec[%d]= %.3lf\n",i,a[i]);
        }
        
        for ( i = 0 ; i < n ; i++)
            for (j = 0; j < m ; j++)
            {
                b[j + (i*m)] = i*(j+1);
                printf("mat[%d]= %.3lf\n", j+(i*m),b[j+(i*m)]);

            }
		
		/*Enviar datos de entrada a los procesos*/
		
		for (i = 1 ; i <= num_procesos ; i++)
		{			
			printf("Soy el proceso %d, estoy enviando a P%d datos para su tarea\n",my_rank, i);
			MPI_Send(&m, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&a, m, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			MPI_Send(&b, m + (m*n), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
		}
		
		for (i = 1 ; i <=num_procesos ; i++)
		{
			MPI_Recv(&c[i-1], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Soy el proceso %d, estoy recibiendo de P%d resultados\n",my_rank, i);
		}
		
		printf("Vector [");
		
		for (i= 0 ; i < n ; i++)
		{
			printf("%.3lf ",c[i]);
		}
		printf("]\n");
	}
	
	if( my_rank != 0)
	{
		MPI_Recv(&m, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&a, m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&b, m + (m*n), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		printf("Soy el proceso %d, estoy recibiendo de P%d \n",my_rank, 0);
		s=0.0;
		i = my_rank - 1;
		for(j=0;j<m;j++)
		{
			s+= a[j]*b[(i*n)+j];		
		}
		printf("Soy el proceso %d, enviando el dato %.3lf \n",my_rank, s);
		MPI_Send(&s, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);	
		
	}	
	
	
	MPI_Finalize();

}

