#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int N =999;
    int total;
    int sum = 0;
    int res=0;
	MPI_Init(&argc, &argv);
	int numP, myId;
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);
	MPI_Comm_size(MPI_COMM_WORLD, &numP);
    if(myId == 0)
    {
        scanf("%d",&total);
    }
    MPI_Bcast(&total, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(myId != 0)
    {
        srand(myId);
        for(int i =0; i < total; i++)
        {
            double x= (double)rand() / (RAND_MAX / 2) - 1;
            double y= (double)rand() / (RAND_MAX / 2) - 1;
            if(x*x+y*y<=1)
                sum++;
        }
    }
    MPI_Reduce(&sum, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(myId == 0)
    {
        printf("%lf",(double)res/(total*(numP-1))*4);
    }

	/* int a, b;
	if(odd == 0) {
		a = -1;
		printf("Process %d sends token %d to process %d\n", myId, a, partner_id);
		MPI_Send(&a, 1, MPI_INT, partner_id, 0, MPI_COMM_WORLD);

		MPI_Recv(&b, 1, MPI_INT, partner_id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %d receives token %d from process %d\n", myId, b, partner_id);
	} else {
		a = 1;
		MPI_Recv(&b, 1, MPI_INT, partner_id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %d receives token %d from process %d\n", myId, b, partner_id);

		printf("Process %d sends token %d to process %d\n", myId, a, partner_id);
		MPI_Send(&a, 1, MPI_INT, partner_id, 0, MPI_COMM_WORLD);
	} */

	MPI_Finalize();
}