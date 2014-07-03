#include <stdio.h>
#include <stdlib.h>
 
#define M 1000 /*Filas*/
#define N 1000 /*Columnas*/
 

int main(int argc, char* argv[]) {
        double a[M] ;
        double b[M*N];
        
        int n, m , i, j;
        
		do{
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
		}while (m == 0 && n == 0);
		
		double s;
		double c[m*n];
		
        for (i = 0; i < n; i++)
		{
			s=0.0;
			for(j=0 ; j<m ; j++)
			{
				s+= a[j]*b[(i*m)+j];   
			}  
			c[i] = s;
		}
 
        printf("\nVector: [");
            for (i = 0 ; i<n ; i++)
                printf("%.1f ",c[i]);
            printf("]\n\n");
}
     

