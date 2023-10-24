#include <stdio.h>
#include <omp.h>

//Check time taken for size 45 and 450000000
//I chose something close to the largest number my compiler can handle for sequential method ':)
#define ARR_SIZE 450000000
int a[ARR_SIZE];

int main() {
    
    int i;

    //Since big time difference will come only for big arrays, i chose a big array
    //Taking input from user for so many elements will take alot of time
    //So im using a simple array with all elements as 1
    for(i=0;i<ARR_SIZE;i++)
        a[i]=1;

    double t1 = omp_get_wtime();

    //SUM USING SEQUENTIAL METHOD
    int sum=0;
    for(i=0;i<ARR_SIZE;i++)
        sum+=a[i];

    double t2 = omp_get_wtime();

    //Time taken is time before and time after loop
    printf("Sequential method took %lf time to find sum=%d\n",t2-t1,sum);

    //SUM USING PARALLEL METHOD
    sum = 0;
    int tid,numt;    //tid is thread id and numt is number of threads

    t1 = omp_get_wtime();
    //If we get n threads, each thread will add sum of array_size/n sum to the final sum
    #pragma omp parallel default(shared) private(i,tid)
    {

        //Note: "#pragma omp for" will automatically divide array into segments and run for loops seperately and do segment sum seperately
        //But that method was too simple and wont show the essence of how we are finding the sum for different segments
        //So, ill show both methods

        numt=omp_get_num_threads();
        tid=omp_get_thread_num();

        //low, high, segment_sum is seperate for each thread bcos i initialize within thread
        int low = tid * (ARR_SIZE/numt);
        int high  = (tid+1) * (ARR_SIZE/numt);
        //this thread will find sum of the (tid+1)th thread segment

        //since ARR_SIZE/numt has some remainder, let remainging elements so to last thread
        if(tid==numt-1)
            high=ARR_SIZE;

        int segment_sum=0;
        //i is also private cos each loop runs seperately
        for(i=low;i<high;i++)
            segment_sum+=a[i];

        //printf("Sum for segement starting at %d and ending at %d is %d\n",low,high,segment_sum);

        //fianlly add sum of all segments to get total sum
        //using critical so that no thread will alter sum until the current thread finished all operations on sum
        // take sum and add to their register, add segment sum to it, keep back the new sum in original address of sum
        //else, maybe thread 1 will take value of sum first, add its segment_sum, 
        //in mean time, all threads finish adding their segment_sum to sum,
        //and we end with the sum we calculated for thread 1 will be pasted in the common sum address
        //so final sum wont be the correct ans, but only sum calculated by thread 1
        //Can explain it more properly if i was physically present ':>
        #pragma omp critical
        sum+=segment_sum;
    }

    t2 = omp_get_wtime();

    //Time taken is time before and time after loop
    printf("Parallel method took %lf time to find sum=%d\n",t2-t1,sum);


    //USING #pragma omp parallel for reduction(+:sum)

    sum = 0;

    t1 = omp_get_wtime();

        #pragma omp parallel for reduction(+:sum)
        for(i=0;i<ARR_SIZE;i++)
            sum+=a[i];

    t2 = omp_get_wtime();

    //Time taken is time before and time after loop
    printf("Parallel method using Pragma omp for took %lf time to find sum=%d",t2-t1,sum);



    return 0;
}


//Bcos critical condition takes a hefty amount of time
//For small size, sequential method was smaller
//But for large size, the time taken by sequential method is much more than the extra time critical condition needs
//So for large size, the parallel method takes lesser time

//This is also the reason why adding segmentsum is better than adding each value to sum directly
//Spending time for critical (number of threads) number of times will take wayy less time compared to 
//spending time for critical MAX_SIZE number of times   (for large numbers)

//For sequential we'll need more space, so use this to execute: g++ -o haha SumOfArray.cpp -fopenmp -mcmodel=large
