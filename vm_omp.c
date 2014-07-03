#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
 
#define M 1000 /*Filas*/
#define N 1000 /*Columnas*/
 
void vm(double *a, int fa,int ca,int lda,double *b,int fb,double *c,int fc);
 
int main(int argc, char* argv[]) {
    static double a[M];
    static double b[M+(N*M)];
    double * c = malloc(sizeof(double)*100000);

    int n, m , i, j,k;
 
    printf("Indicar Matriz\n");
    do{
        printf("Numero de Filas: ");
        scanf("%d", &m);
        printf("Numero de Columnas: ");
        scanf("%d", &n);
        if((n>N) || (m>N)){
            printf("Error, tamano demasiado grande.\n");
        }
    }while((n>N) || (m>N));
 
     
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
    vm(a,m,n,m,b,m,c,m);
 
    printf("\nVector resultante: [");
        for (i = 0 ; i<n ; i++)
            printf("%.1f ",c[i]);
        printf("]\n\n");
   
   free(c);

        return 0;
 
    }   
 
    void vm(double *a, int fa,int ca,int lda,double *b,int fb,double *c,int fc)
{
    int i, j, iam, np; double s;

        #pragma omp parallel default(shared) private(i,j,s,iam,np)  
        {
            np = omp_get_num_threads();
            iam = omp_get_thread_num();
            printf("Hello from thread %d out of %d\n",iam,np);
            for (i = 0; i < ca; i++)
            {
                s=0.0;
                //printf("\nSoy el hilo %d y voy a calcular el elemento %d",omp_get_thread_num(),i); 
                for(j=0;j<fa;j++)
                {
                    s+= a[j]*b[i*lda+j];    
                }   
                c[i] = s;
                printf("%.1f\n", c[i]);
            }

    }
  
}
