#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	int numP, myId;
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);
	MPI_Comm_size(MPI_COMM_WORLD, &numP);

	if(numP % 2 != 0) {
		if(!myId) {
			printf("ERROR: the number of ping-pong processors must be even number!\n");
		}
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	int partner_id;
	int odd = myId & 1;

	if(odd) {
		partner_id = myId - 1;
	} else {
		partner_id = myId + 1;
	}

	int a, b;
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
	}

	MPI_Finalize();
}