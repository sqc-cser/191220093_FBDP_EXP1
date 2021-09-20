#define N 100000000
#define a 10
#define b 100
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    clock_t start, finish;
    MPI_Status status;
    double  duration;
    int myid,numprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    --numprocs; /*数据分配时除去0号主节点*/
    int i;
    double local=0.0, dx=(double)(b-a)/N; /* 小矩形宽度 */
    double inte, x;
    if(myid==0) /* 规约所有节点上的累加和并送到主节点0 */
    {   /* 主节点打印累加和*/
        start = clock();
        double sum =0 ;
        for(int source = 1;source<=numprocs;++source){
            MPI_Recv(&inte, 1,MPI_DOUBLE, source,99,MPI_COMM_WORLD,&status);
            printf("从%d 进程获取返回值 %lf\n",source,inte);
            sum += inte;
        }
        printf("The integal of x*x*x in region [%d,%d] =%16.15f\n", a, b, sum);
        finish = clock();
        duration = (double)(finish - start)/CLOCKS_PER_SEC;
        printf( "花费时间 %f seconds\n", duration );
    }
    else
    {
        for(i=myid;i<N;i=i+numprocs) /*根据节点数目将N个矩形分为图示的多个颜色组*/
        { /*每个节点计算一个颜色组的矩形面积并累加*/
            x = a + i*dx +dx/2; /* 以每个矩形的中心点x值计算矩形高度 */
            local +=x*x*x*dx; /* 矩形面积 = 高度x宽度=y*dx */
        }
        MPI_Send(&local, 1, MPI_DOUBLE, 0, 99,MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
