#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main(int argc,char* argv[]){
    int b = atoi(argv[1]);
    int nthreads = atoi(argv[2]);
    double start = atoi(argv[3]);
    double end = atoi(argv[4]);
    omp_set_num_threads(nthreads);
    int n ;
    double count[20];
    scanf("%d",&n);
    for(int t = 0;t < n; t++)
        scanf("%lf",&count[t]);
    double step = (end-start)/b;

    int note[2*n];
    for(int t = 0;t < 2*n; t++)
        note[t]=0;
#pragma omp parallel
	{
		int thread_id = omp_get_thread_num();
        int element_count = n / nthreads;
        int start_num = thread_id *element_count;
        int end_num = (thread_id+1) *element_count;
        for(int i = start_num;i<end_num;i++)
        {
            int index = (int)(count[i]-start)/step;
            #pragma omp atomic
                note[index]++;
        }
	}
    for(int t = 0;t < b; t++)
    {
        printf("From %.3f to %.3f: %d\n", start + t * step, start + t * step+ step,note[t]);

    }
  
  return 0;
}

