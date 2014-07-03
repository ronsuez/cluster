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
			c[N],				/*Vector Fila resultante C*/			
			vaux[N];
	
	double	s,col_proc;
	
	int 	size, 				/*Cantidad de procesos*/
			my_rank,			/*Numero de proceso*/
			num_procesos,		/*Cantidad de procesos 'workers'*/
						/*columnas por proceso*/
			aux,			/*Columnas restantes*/
			m ,n,				/*Fila y columna*/
			i,j,k,li,ls;				/*Variables de control*/
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
				//m = 50;	/* cantidad de filas */
		//n = num_procesos;

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
		do{
			printf("Procesos a Ejecutar %d\n",num_procesos);
			printf("Indica filas y columnas\n");
			scanf("%d %d",&m,&n);

			for (i = 0 ; i < m; i++)
			{
				a[i] = i+1; 
				//printf("vec[%d]= %.3lf\n",i,a[i]);
			}
			
			for ( i = 0 ; i < n ; i++)
				for (j = 0; j < m ; j++)
				{
					b[j + (i*m)] = i*(j+1);
				 //printf("mat[%d]= %.3lf\n", j+(i*m),b[j+(i*m)]);

				}

			col_proc = n / num_procesos;
			printf("\n**********%.0lf\n************\n",col_proc);
		}while (n%num_procesos != 0);
		
		getchar();
		/*Enviar datos de entrada a los procesos*/
		
		for (i = 1 ; i <= num_procesos ; i++)
		{			
			//printf("Soy el proceso %d, estoy enviando a P%d datos para su tarea\n",my_rank, i);
			MPI_Send(&m, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&a, m, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			MPI_Send(&b, (m*n), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			MPI_Send(&col_proc, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
		}
		
		for (i = 1 ; i <=num_procesos ; i++)
		{
			/*MPI_Recv(&li, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&ls, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);*/
			MPI_Recv(&aux, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&vaux, aux, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			
			printf("Soy el proceso %d, ini %.0lf fin %.0lf\n",my_rank,(i-1)*col_proc, (i)*col_proc);
			for ( j = ((i-1)*col_proc) ; j < ((i)*col_proc) ; j++)
			{
				c[j] = vaux[j-(int)((i-1)*col_proc)];
			}
			//printf("Soy el proceso %d, estoy recibiendo de P%d resultados\n",my_rank, i);
		}
		
		printf("\n\nVector [");
		
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
		MPI_Recv(&b, (m*n), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&col_proc, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		//printf("Soy el proceso %d, estoy recibiendo de P%d \n",my_rank, 0);
		int i, j,cont;
		double  min, max;
		
		min = (my_rank - 1) * col_proc;
		max = my_rank * col_proc;
		
		
		printf("Soy el proceso %d, ini %.0lf fin %.0lf\n",my_rank, min, max);
		double s[(int)(max-min)];
		cont = 0;
		for (i = min ; i < max ; i++)
		{
			for( j = 0 ; j < m; j++ )
			{
				s[cont] += a[j] * b[ (i*m) + j ];		
			}
			cont++;
		}
		cont++;
		//printf("Soy el proceso %d, enviando el dato\n",my_rank);
		MPI_Send(&cont, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(&s, cont, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		
	}	
	
	
	MPI_Finalize();

}

