#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    clock_t start, finish;
    double  duration;
    MPI_Status status;
    int myid, P, source, C=0,numprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    int N;
    double SqrtSum=0.0,d,local=0;
    if(myid==0){ // 发送数据N
        printf("请输入N\n");
        scanf("%d",&N);
        start = clock();
    }
    MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);
//----------------------------------------------------------------
    if (myid!=0){
        start = clock();
    }
    for(int i = myid; i<N;i+=numprocs)
    {
        local += sqrt(i*(i+1));
    }
    finish = clock();
    duration = (double)(finish - start)/CLOCKS_PER_SEC;
    printf("%d进程花费时间 %f seconds\n", myid,duration );
    /* MPI_Reduce 改写！！！*/
    MPI_Reduce(&local,&SqrtSum,1,MPI_DOUBLE,MPI_SUM, 0, MPI_COMM_WORLD);
    /* MPI_Reduce 改写！！！*/
    if(myid == 0){
        printf("SqrtSum: %lf\n",SqrtSum);
        finish = clock();
        duration = (double)(finish - start)/CLOCKS_PER_SEC;
        printf("total花费时间 %f seconds\n", duration );
    }
    MPI_Finalize();
}
