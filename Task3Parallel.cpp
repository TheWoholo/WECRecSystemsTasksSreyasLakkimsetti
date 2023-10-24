#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <ctime>

#define N 10

double A[N][N];
/*double A[N][N] = {
    {3.0, 2.0, -4.0, 7.0},
    {2.0, 3.0, 3.0, -8.0},
    {5.0, -3.0, 1.0, -2.0},
    {4.0, 9.0, -8.0, 11.0}
};*/
double A1[N][N];

void getCofactor(double A[N][N], double temp[N][N], int p, int q, int n) {
    int i = 0, j = 0;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (row != p && col != q) {
                temp[i][j++] = A[row][col];
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

double determinant(double A[N][N], int n) {
    if (n == 1) {
        return A[0][0];
    }

    double det = 0;
    double temp[N][N];


    #pragma omp parallel for reduction(+ : det) private(temp)
    for (int f = 0; f < n; f++) 
    {
        getCofactor(A, temp, 0, f, n);
        det += pow(-1,f) * A[0][f] * determinant(temp, n - 1);
    }

    return det;
}

int main()
{

    //Set A as identity matrix, cos matrix is big, cant take user input, use already known matrix
    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            if(i==j)
                A[i][j]=1;
            else    
                A[i][j]=0;
        }
    }

    time_t start, end; 

    time(&start);

    double det = determinant(A, N);

    if (det == 0) {
        printf("Inverse doesn't exist. Matrix is singular.\n");
        return 0;
    }

    int sign = 1;
    double temp[N][N];

    //bcos all A1[j][i] are independant of other elements and operations, we can do parallel programming
    #pragma omp parallel for collapse(2) private(temp)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
                getCofactor(A, temp, i, j, N);
                A1[j][i] = pow(-1,i+j) * (determinant(temp, N - 1) / det);
            
        }
    }

    time(&end);

/*
    printf("Inverse of A:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%lf\t", A1[i][j]);
        }
        printf("\n");       
    }
*/
     double time_taken = double(end - start);
        //Time taken is time before and time after loop
    printf("Parallel method using Pragma omp for took %lf time\n",time);

    return 0;
}
