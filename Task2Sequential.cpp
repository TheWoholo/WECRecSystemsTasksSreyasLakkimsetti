#include <stdio.h>
#include <omp.h>
#include <ctime>

#define N 100

/*double A[N][N] = {
        {3.0, 2.0, -4.0, 7.0},
        {2.0, 3.0, 3.0, -8.0},
        {5.0, -3.0, 1.0, -2.0},
        {4.0, 9.0, -8.0, 11.0}
    };*/
double A[N][N];
double L[N][N];
double U[N][N];

void A00(int from, int b) 
{
    int i,j,k;
    int N1=b+from;

    //LU Factorisation

    //Let us assume that we dont encounter 0 rows and no row exchanges are required
    //We perform operations row wise
    for(i=from;i<N1;i++)
    {
        //Assuming pivot is A[i][i] always

        //Write this row onto U[i]
        for(j=from;j<N1;j++)
        {
            U[i][j]=A[i][j];
        }

        //Make all ith colums 0
        for(j=i+1;j<N1;j++)
        {
            //Now for the jth row, we need to make A[j][i]=0
            //Set L[j][i] as A[j][i]/A[i][i]
            L[j][i]=A[j][i]/A[i][i];

            //Convert each element in jth row to A[j][k]=A[j][k]-L[j][i]*A[j][k]
            for(k=i;k<N1;k++)
            A[j][k]-=L[j][i]*A[i][k];
        }

    }
}

void U01(int from, int b)
{
    int i,j,k;
    for(i=from+b;i<N;i++)
    {
        for(j=from;j<from+b;j++)
        {
            U[j][i]=A[j][i];

            for(k=from;k<j;k++)
            U[j][i]-=L[j][k]*U[k][i];
        }
        
    }
}

void L10(int from, int b)
{
    int i,j,k;
    for(i=N-1;i>=from+b;i--)
    {
        for(j=from+b-1;j>=from;j--)
        {
            L[i][j]=A[i][j];

            for(k=from+b-1;k>j;k--)
            L[i][j]-=L[i][k]*U[k][j];

            L[i][j]/=U[j][j];
        }
        
    }
}

void A11(int from, int b)
{
    int i,j,k;
    for(i=from+b;i<N;i++)
        for(j=from+b;j<N;j++)
        {
            for(k=from;k<from+b;k++)
            A[i][j]-=L[i][k]*U[k][j];
        }
}

void func(int from, int to)
{
    A00(from,to-from);
    U01(from,to-from);
    L10(from,to-from);
    A11(from,to-from);
}

int main()
{
    int i,j,k;
    //double A[N][N];

    
    //Since N is very large we'll just use an identity matrix
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
        {
            if(i==j)
                A[i][j]=1.0;
            else
                A[i][j]=0.0;
        }
    

    //Initialize U
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            U[i][j]=0;

    //Initialize L
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
        {
            if(i==j)
                L[i][j]=1.0;
            else
                L[i][j]=0.0;
        }

        time_t start, end; 
    time(&start);

    int numt, tid, starting_index;

    int chunk_size=11; //Eg. of number of threads
    int i1;
    for(i1=0;i1<N-chunk_size;i1+=chunk_size)
    {
        func(i1,i1+chunk_size);
    }
    func(i1,N);

    time(&end);


    //Print L and U

    printf("L:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%lf\t", L[i][j]);
        }
        printf("\n");
    }

    printf("U:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%lf\t", U[i][j]);
        }
        printf("\n");
    }

    double time_taken = double(end - start);
        //Time taken is time before and time after loop
    printf("Sequential method took %lf time\n",time);


    return 0;
}
