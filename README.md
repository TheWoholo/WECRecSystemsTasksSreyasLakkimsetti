# WECRecSystemsTasksSreyasLakkimsetti

NOT DONE!!!!!

I chose the Parallel Computing Task

Problem Statements:
1.Each of these tasks are standalone tasks.
Sum of an Array:
Write a parallel program to calculate the sum of an array.
2.LU Decomposition:
Write a parallel program to perform LU Decomposition of an array. 
3.Inverse of a Matrix:
Write a parallel program to find the inverse of a matrix. 


To execute the codes
this it the terminal method
g++ -o haha LUParallelI.cpp -fopenmp -mcmodel=large
and then
./haha



for task1
when N=450000000, output is,  
Sequential method took 0.943000 time to find sum=450000000
Parallel method took 0.168000 time to find sum=450000000
Parallel method using Pragma omp for took 0.155000 time to find sum=450000000

when N=45
Sequential method took 0.000000 time to find sum=45
Parallel method took 0.004000 time to find sum=45
Parallel method using Pragma omp for took 0.000000 time to find sum=45

I used comments to explain the code

for task2
I used to block method where i execute some parts sequentially and some parts parallely
Depending on the chunk size, either sequential or parallel methods for the subparts are better

for task3

Note that, we can perform task3 using a code very similar to task2
we can use GaussJordan Method where we perform LU Decomposition on A, 
then take transpose of A and L and perform LU Decomposition again, 
divide each row of L by the diagonal element in final matrix A
and the inverse will be the the transpose of L or L
