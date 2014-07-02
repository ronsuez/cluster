#include <stdio.h>
#include <stdlib.h>
 
#define M 100 /*Filas*/
#define N 100 /*Columnas*/
 
void vm(double *a, int fa,int ca,int lda,double *b,int fb,double *c,int fc);
 
int main(int argc, char* argv[]) {
        double vec[M];
        double mat[M+(N*M)];
        double c[N];
        int n, m , i, j;
 
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
 
        for(i=0; i<m; i++){
            printf("vec[%d]= ", i);
            scanf("%lf", &vec[i]);
        }
 
        for(i=0; i<n; i++){
            for(j=0; j<m; j++){
                printf("\nmat[%d][%d]= ",j, i);
                scanf("%lf", &mat[j + (i*m)]);
            }
        }
 
        vm(mat,m,n,m,vec,m,c,m);
 
        printf("\nVector resultante: [");
            for (i = 0 ; i<n ; i++)
                printf("%.1f ",c[i]);
            printf("]\n\n");
}
     
void vm(double *a, int fa,int ca,int lda,double *b,int fb,double *c,int fc)
{
    int i, j; double s;
    for (i = 0; i < ca; i++)
    {
        s=0.;
        for(j=0;j<fa;j++)
        {
            s+= b[j]*a[i*lda+j];    
        }   
        c[i] = s;
    }
}
